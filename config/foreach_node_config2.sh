#! /usr/bin/bash

if [ "$#" -eq 3 ]; then
	docker exec -i $1 /bin/bash 'Docker_MPI/setup.sh' $2 $3

#	spawn docker exec -i $1 ssh-copy-id -f -i /home/cpd2021/.ssh/id_rsa.pub master -o User=cpd2021 -o StrictHostKeyChecking=no

#	docker exec -it $1 ssh-copy-id -i /home/cpd2021/.ssh/id_rsa.pub node01 -o User=cpd2021 -o StrictHostKeyChecking=no
	
fi


