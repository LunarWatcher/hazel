echo "Using username ${HAZEL_USER:=hazel}"

cd /opt/hazel

sudo -u $HAZEL_USER git pull
cd build 
# Need to set HOME explicitly in case the user doesn't have a home set. This really should be handled better
# Could also use /tmp here, but I don't know if that's necessarily desirable
if ! sudo -u $HAZEL_USER HOME="/home/$HAZEL_USER" make -j $(nproc) install; then
    echo "Failed to install."
    echo "Breaking changes notes:"
    echo "* Installations prior to 2026-02-12 do not use conan, and has more system packages enabled."
    echo "  To enable conan, you can run:"
    echo '    sudo -u ${HAZEL_USER:-hazel} cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/opt/hazel/dist/ -DHAZEL_ENABLE_CONAN=ON -DCMAKE_POLICY_VERSION_MINIMUM=3.5'
    echo "  And then run the update script again. Note that you need conan installed."
    exit 1
fi
sudo systemctl restart hazel


