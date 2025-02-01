
#!/bin/bash
echo "Welcom to swtich case program"
case ${1,,} in
	bhakti | kavya )
		echo "You are boss of this system"
		;;
	help)
		echo "Just press the username bruh"
		;;
	*)
		echo "Cmmo'n It's not that Hard"
		;;
esac

