# 42-Irc

The ft_IRC project is a 42 School project consisting in developping a simple IRC server implemented in C++98. It supports multiple clients and basic IRC commands, such as authentication, setting nicknames and usernames, joining channels, sending and receiving private messages, and handling operators. The server does not support server-to-server communication or a dedicated client.

## Getting Started

These instructions will help you compile and run the ft_IRC server on your local machine.

### Prerequisites

- A C++98 compatible compiler (e.g., GCC or Clang)
- GNU Make

### Compiling the server

1. Clone the repository

2. Navigate to the project directory

3. Compile the server

### Running the server

To start the ft_IRC server, use the following command : `./ircserv <port> <password>`
- `<port>`: The port number on which the IRC server will listen for incoming IRC connections.
- `<password>`: The connection password, which will be required by any IRC client trying to connect to the server.

For example:

`./ircserv 6667 my_password`

## Using the server

1. Choose an IRC client as a reference, tested with HexChat for the dev. (e.g., Weechat, [LimeChat](http://limechat.net/mac/), [HexChat](https://hexchat.github.io/), [mIRC](https://www.mirc.com/), or [Irssi](https://irssi.org/)).

2. Configure your IRC client to connect to the ft_IRC server using the IP address or hostname of the machine running the server, along with the port number and password specified when starting the server.

3. Once connected, you can use your IRC client to authenticate, set a nickname and username, join channels, send and receive private messages, and manage operators.
