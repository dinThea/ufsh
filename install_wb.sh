apt-get install -y libscm-dev texinfo
cd wb
make all
make install
ldconfig
ls -lh /usr/local/lib/*wb* | wc -l
# outputs 10 on my machine
ls -lh /usr/local/include/*wb* | wc -l
# outputs 17 on my machine
