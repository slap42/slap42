# Slap42

Slap42 is a game I am building live on Twitch.

I know my code is terrible and I don't care. Feel free to roast me.

If there's a feature you want added or a bug you want fixed, open a github issue or mention it in the twitch chat.

Big shout out to kutobu__, been there since day one, a true believer in the process 🥐🇫🇷🥖🙏💯

p0lloc is cool too I guess

ImRiux is officially entitled to a 3% revshare for making an actual contribution to the code

## How to play

### Downloading the Game

You can download the game here: https://slap42.itch.io/slap42

I'll release updates on the itch.io page as often as I can remember to. Updates for the Windows version will release much more often than macOS and Linux, because I'm mostly working on a Windows machine. But I'll try to keep them up to date.

Since I am updating the game often, things will break all the time. Sorry.

### Compiling The Game

Alternatively, you can compile the game from source yourself.

The build system is cmake and compiling the game requires C++20.

Windows, macOS, and Linux are supported.

### Hamachi Network

To join the game while I'm live, you can join this Hamachi network:

Network: slap42

Password: slap42

Please don't write anything racist in the chat and get me banned.

## Links

Github: https://github.com/slap42/slap42

Itch.io: https://slap42.itch.io/slap42

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
- Delta time
- Camera mouse control on a Linux VM goes crazy and will sometimes cause a segfault
- Terrain flickering bug reported on Windows
