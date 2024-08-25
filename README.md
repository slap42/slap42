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

### Running the Game

The game expects to be run from a folder containing another folder called `res`. The `res` folder contains all the resources needed to run the game - images, shaders, etc.

If you're on Windows it should be enough to just unzip the folder you downloaded from itch.io and click `Slap42.exe`, making sure not to move any of the files or folders from their original locations.

If running from a terminal or command prompt, make sure the working directory is the folder containing the executable file and the res folder.

### Warnings on Windows

If you download the game from itch.io you will probably get warnings about the executable not being from a trusted source. I haven't paid Microsoft for a signing key yet, so this warning will pop up until I do so. If you trust me you can choose `more options` then `run anyway`. If you don't trust me that's okay, you can compile from source yourself or go look at the code to make sure I'm not doing anything dodgy.

### Controls

- WASD - Move
- Mouse or Arrow Keys - Turn the camera
- T - Open chat
- Esc - Close chat / unlock mouse

### Hamachi Network

To join the game while I'm live, you can join this Hamachi network:

Network: `slap42`

Password: `slap42`

Please don't write anything racist in the chat and get me banned.

## Links

Github: https://github.com/slap42/slap42

Itch.io: https://slap42.itch.io/slap42

Twitch: https://www.twitch.tv/slap42dev

Youtube: https://www.youtube.com/@Slap42Dev

## Notes

If running in a VM, make sure mouse integration is turned on or cursor input won't work

## Todo List:

### New Features

- Physics
- Grass and trees
- Model loading
- IP whitelists/blacklists for servers
- Usernames
- Joystick controls
- Controls rebinding
- Serialize preferences including username, fov, and render distance
- ImGui styling
- Lighting
- Skybox
- Day / night cycle

### Backlog

#### Cleanup

- Figure out if there is a way to show an error message if the connection fails because the server is at capacity

#### "It Works On My Machine"

- Terrain flickering bug reported on Windows
- Might be a race condition causing enet_peer_send to fail?
