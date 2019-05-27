solve: main.o
	g++ -o $@ -O3 -Wall -Wextra -std=c++17 $^

%.o: %.cpp
	g++ -o $@ -c -O3 -Wall -Wextra -std=c++17 $<

main.o: menu.csv

menu.csv:
	wget https://raw.githubusercontent.com/hodaka0714/saize_calory_maxmization/master/menu.csv
