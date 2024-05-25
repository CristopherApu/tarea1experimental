all:
		g++ receptor.cpp -o receptor funciones.cpp -lwiringPi
		g++ emisor.cpp -o emisor funciones.cpp -lwiringPi
		g++ clock.cpp -o clock -lwiringPi
emisor:
		./emisor
receptor:
		./receptor
reloj:
		sudo ./clock
todo:
		sudo ./clock &
		sudo ./receptor &
		sudo ./emisor