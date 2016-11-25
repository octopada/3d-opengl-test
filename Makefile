all: adventure

adventure: main.cpp
adventure: resource_manager.cpp
adventure: sprite_renderer.cpp
adventure: text_renderer.cpp
adventure: texture.cpp
adventure: shader.cpp
adventure: camera.cpp
adventure: glad.c
adventure: game.cpp
adventure: game_level.cpp
adventure: game_object.cpp
	g++ -o $@ $^ -lGL -lglfw -ldl -lSOIL -lfreetype -std=c++11 -w -g

clean:
	rm adventure
