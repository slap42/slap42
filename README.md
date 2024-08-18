# Slap42

Slap42 is a game I am building live on Twitch.

Big shout out to kutobu__, been there since day one, a true believer in the process 🥐🇫🇷🥖🙏💯

p0lloc is cool too I guess

ImRiux is officially entitled to a 3% revshare for making an actual contribution to the code

## How to play

Right now the only way to play is to compile the game from source yourself.
The build system is cmake and compiling the game requires C++20.
Windows, macOS, and Linux are supported.

I'll be uploading .exe files etc to itch.io at some point when more features are implemented.

## Hamachi Network

To join the game while I'm live, you can join this Hamachi network:

Network: slap42

Password: slap42

## Links

Github: https://github.com/slap42/slap42

Itch.io: https://slap42.itch.io/

Twitch: https://www.twitch.tv/slap42dev

Youtube: https://www.youtube.com/@Slap42Dev

## Todo List:

### New Features

- Physics
- Grass and trees
- Chat system

### Backlog

- When starting a server, check whether the port is available. If it is not, use another port.
- Allow choosing a port when joining a server?
- Allow setting FOV
- Make trying to join a server happen asynchronously
- Allow configuring the number of players able to join a server
- IP whitelists/blacklists for servers
- Cleanup the switch statements in Client/Server to make them easier to read
- Point logs at an in-game console instead of stderr/stdout
