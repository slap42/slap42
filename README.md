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

However, since I don't have signing keys, etc. you will probably see a whole load of warnings saying the executable file cannot be opened, it comes from an untrusted source, and so on. You can either follow the process for your platform to run it anyway (On Windows `more options` > `run anyway`), or you can compile it from source. At some point I will start signing binaries, but this isn't a big priority right now.

I'll release updates on the itch.io page as often as I can remember to. Updates for the Windows and macOS versions will release much more often than Linux because that's where I do most of my development.

Since I am updating the game often, things will break all the time. Sorry.

### Compiling The Game

Alternatively, you can compile the game from source yourself.

The build system is cmake and compiling the game requires C++20.

Windows, macOS, and Linux are supported.

### Running the Game

The game expects to be run from a folder containing another folder called `res`. The `res` folder contains all the resources needed to run the game - images, shaders, etc.

If you're on Windows it should be enough to just unzip the folder you downloaded from itch.io and click `Slap42.exe`, making sure not to move any of the files or folders from their original locations.

If running from a terminal or command prompt, make sure the working directory is the folder containing the executable file and the res folder.

### Controls

- WASD, LShift, Space - Move
- Mouse or Arrow Keys - Turn the camera
- T - Open chat
- Esc - Close chat / Open menu

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
- Better lighting
- Skybox

### Backlog

#### Cleanup

- Figure out if there is a way to show an error message if the connection fails because the server is at capacity
- Generate meshes on another thread
- Leaves

#### "It Works On My Machine"

- Terrain flickering bug reported on Windows
- Might be a race condition causing enet_peer_send to fail?
