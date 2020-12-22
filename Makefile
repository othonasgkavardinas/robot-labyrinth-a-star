CC=gcc

install: main.c
	$(CC) -o robot_labyrinth_a_star main.c

run: robot_labyrinth_a_star
	./robot_labyrinth_a_star

clean: robot_labyrinth_a_star
	rm robot_labyrinth_a_star