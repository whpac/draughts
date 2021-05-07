# Draughts
This repository houses a draughts game, which was made as a project for
*Low-level programming in C* classes. It has two different modes of
operation &mdash; textual and graphical.

Graphical mode requires the Allegro library (I used the 5.2.7 version).

## Usage
By default, the textual mode is used only when it's impossible to initialize
a window with the board. You can also enforce it by invoking the game with
`text` command-line argument.

    draughts.exe text

### Controls in the graphical mode
Right after launching, you are going to be asked whether to start a new game
or to load an older save. There's a limitation of 16 characters in file name.

When the board is ready, use arrow keys to move the cursor. You can move only
those pawns that are surrounded by a green frame when selected. Press the Enter
key to choose a pawn to be moved and then select a destination field. Game marks
the legal destinations with gray dots.

You are obliged to perform a move that kills as many opponent's pawns as possible.
If there are more equally good moves, you are free to choose any of them. It's
completely acceptable to kill a pawn backwards.

To transform a man into a king, you have to finish his move at the final row. The
transition is marked by an additional circle on the pawn symbol. Please note that
stepping through the final row during a complex kill is not sufficient for the
transformation to take place.

There is an option to undo as many moves as you wish. Just press U and the recently
made move will be reversed. It's possible to undo the game back to the initial state.
That's the case also when the game was loaded from a file.

The game continues as long as both players are able to move. However, you can exit
at any time (pressing Esc). Upon exiting, you will be asked whether to save the
game. Again, there is a 16-letter limit of the file name length. If you don't want
to save it, just leave the name input empty.

### Controls in the textual mode
Just as before, the first thing the game will ask you for is whether to load a save.
However, there is a length limit of 64 characters now. You can load any file generated
by the game, no matter whether it was an outcome of a graphical game or a textual one.

To move pawns, you have to type a command. The basic one is `m` which accepts four parameters:

    m <from_row> <from_column> <to_row> <to_column>

They describe the source field (the first two) and the destination (the second two).
Rows and columns are numbered at the edges of the board for your comfort. You can move
pawns only to fields marked by a dot.

An ordinary pawn is marked by a small letter 'b' or 'w' (black or white) whereas kings
are denoted with capital letters ('B' or 'W').

There is also an undo command `u` which accepts no arguments. It's function is exactly
the same as in the graphical mode.

You can always invoke an in-game command list using the question mark `?`. To quit the
game, type `q` and press Enter. The game is ended automatically when one of the players
loses all their pawns.

After choosing to exit the game, you will be asked whether to save it. The file name
length limit here is 64 letters too.

## Build process (Windows)
Please ensure that you have the Allegro library installed. This project utilizes a
makefile, so you will need `make` too. It is sufficient to run `make` in order to
compile the game.

There are two additional rules, namely `make run` and `make run-text` which, apart
from building the sources, run the game, respectively, in GUI and in the text mode.

## Source files
The source files are organized into four main directiories. The `board/` houses the
main game logic and rules associated with moves and kills. There are also declarations
of structures for the board and pawn. This code handles move undoing too. Then, there's
`log/` catalog which contains code responsible for logging the user's activities and
retrieving the saved game state. Instructions related to the user interface are saved
in `interface/` directory, further subdivided into `gui/` and `text/`.

The last folder is `data/` which contains some data structures that are used by the game,
such as queue or list.