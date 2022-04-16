#! /usr/bin/bash

if [ "$#" -ne 1 ]; then
    echo "Illegal number of parameters"
    exit 0
fi

docker start $1 > /dev/null
docker exec -it $1 /usr/bin/git clone 'https://github.com/delucap/Docker_MPI.git' > /dev/null
docker exec -it $1 /bin/bash 'Docker_MPI/user.sh' > /dev/null
docker exec -it $1 chown -R cpd2021:cpd2021 /Docker_MPI

# get ip from nodes
ipaddr=`docker exec -it $1 hostname -I`
echo $ipaddr

