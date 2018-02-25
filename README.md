# Crypto experiments repository

Here is some of my crypto primitive implementations. I wrote them for learning purposes.
**DON'T EVER USE CODE FROM THIS REPO IN REAL APPLICATION.** There is plenty of good, fast and bug-free crypto libraries.

List of implemented algos:

- AES. Implemented for 2 purposes: to understand how AES works and how AES-NI can be used(the latter is not implemented yet)
- lyra2z: blake256 + lyra2 with params: bitrate - 96 bytes, capacity - 32 bytes, key length - 32 bytes, password length - 32 bytes, salt length - 32 bytes, timeCost - 8, nRows - 8, nCols - 8. password and salt values are the same. "Lyra2 with parameters" is the most interesting here, blake256 completely taken from sph lib. Lyra2 code written here with a goal to port it to opencl later.
