echo "Using username ${HAZEL_USER:=hazel}"

sudo -u $HAZEL_USER git pull
cd build 
sudo -u $HAZEL_USER make -j $(nproc) install
sudo systemctl restart hazel


