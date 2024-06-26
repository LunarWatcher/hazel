#!/usr/bin/bash
set -x
set -e

# TODO:
# * use venv to get CMake >= 3.28
# * More checks for existence prior to running stuff


cd /opt
sudo mkdir hazel

# Gotta love shell
echo "Using username ${HAZEL_USER:=hazel}"

# This command failure is checked; re-allow bash to continue even on error
set +e
id -u $HAZEL_USER
if [[ $? != 0 ]]; then
    set -e
    sudo useradd --system --user-group --home-dir / --shell /sbin/nologin $HAZEL_USER
else
    set -e
    set +x
    echo "User $HAZEL_USER already exists. It's assumed this is correct."
    echo "If it isn't, you have 10 seconds to press ctrl-c to abort the installation."
    echo "Using an existing user is fine, but may have security implications in the event of"
    echo "a security breach. It's strongly recommended to use a dedicated user for the server"
    echo "NOTE: If you're rerunning the install script to update hazel, and the existing user _is_ for this server, don't."
    echo "Use /opt/hazel/scripts/update.sh instead."
    set -x
    sleep 10
fi

sudo chown -R $HAZEL_USER /opt/hazel
sudo -u $HAZEL_USER git clone https://github.com/LunarWatcher/hazel
cd /opt/hazel

# TODO check if debian and run the dependency script

sudo -u $HAZEL_USER mkdir build
cd build
sudo -u $HAZEL_USER cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/opt/hazel/dist/ 
sudo -u $HAZEL_USER make -j $(nproc)
sudo -u $HAZEL_USER make install

# TODO: check if nginx is installed first
cat <<EOF | sudo tee /etc/nginx/conf.d/hazel.conf
# Hazel
server {
    listen 443 ssl http2;
    server_name hazel.${HAZEL_DOMAIN:-FIXME};
    ssl_certificate         ${HAZEL_CERT:-FIXME};
    ssl_certificate_key     ${HAZEL_CERT_KEY:-FIXME};

    location / {
        proxy_set_header   X-Real-IP \$remote_addr;
        proxy_set_header   X-Forwarded-For \$proxy_add_x_forwarded_for;
        proxy_set_header   Host \$host;
        proxy_pass         http://localhost:6906/;
        # Required to prevent downgrades during redirects
        # This is caused by an oddity in how Crow handles redirects. See
        #     https://github.com/CrowCpp/Crow/blob/ad337a8a868d1d6bc61e9803fc82f36c61f706de/include/crow/http_connection.h#L257
        # for the code in question
        proxy_redirect     http://$host/ https://$host/;
        proxy_http_version 1.1;
        proxy_set_header   Upgrade \$http_upgrade;
        proxy_set_header   Connection "upgrade";

        # Required for HTTP basic auth services
        proxy_set_header   Authorization \$http_authorization;
        proxy_pass_header  Authorization;
    }
}
EOF

cd /opt/hazel
sudo cp etc/systemd/system/hazel.service /etc/systemd/system/hazel.service
sudo mkdir -p /etc/hazel
sudo cp etc/hazel/hazel.conf.template /etc/hazel/hazel.conf.template

echo "A config file has been saved to /etc/nginx/conf.d/hazel.conf"
echo "Edit it and restart nginx to get started."
echo "Also make sure to create /etc/hazel/hazel.conf. See /etc/hazel/hazel.conf.template for a starter"
echo "When you're ready to start hazel, run sudo systemctl enable hazel && sudo systemctl start hazel"
