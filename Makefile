CFLAGS += -std=c99
CFLAGS += -Wall
CFLAGS += -fPIC
CFLAGS += -O3
CFLAGS += -shared
CFLAGS += `pkg-config --cflags pidgin`

NAME=panotify

all: $(NAME)

$(NAME): $(NAME).c
	$(CC) $(CFLAGS) -o $(NAME).so $(NAME).c
	strip $(NAME).so

clean:
	rm -f $(NAME).so

deploy: all
	cp ./$(NAME).so ~/.purple/plugins/

undeploy:
	rm -f ~/.purple/plugins/$(NAME).so