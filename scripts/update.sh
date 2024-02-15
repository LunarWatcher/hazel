echo "Using username ${HAZEL_USER:=hazel}"

sudo -u $HAZEL_USER git pull && cd build && make -j $(nproc) install


