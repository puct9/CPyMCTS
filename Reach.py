"""
This script is designed to function as a higher level interface
to an end user & programmer. It also handles input checking and such
NO functionality is removed because of this
"""
from typing import Tuple
from loader import Board as Board_, search as search_


class Board:

    def __init__(self, size: int = 19) -> None:
        if size <= 0:
            raise ValueError("Bad value for size.")
        if size > 19:
            raise ValueError("Size may not be larger than 19.")
        self.board = Board_(size)

    # Read only properties and (magic) methods
    def __str__(self) -> str:
        return self.board.string.replace('-1', 'X ').replace('1', 'O')

    @property
    def size(self) -> int:
        """Returns the size of the board. If 19x19 then will return 19"""
        return self.board.size

    @property
    def move_count(self) -> int:
        """Moves that have been played since game start minus undone moves"""
        return self.board.move_count

    def check_win(self) -> int:
        """Returns -1 for P1 win, 0 for no win, 1 for P2 win"""
        return self.board.check_win()

    # The remainder of the methods to interface with Board_
    def reset(self) -> None:
        """Resets to a new instance of `Board`"""
        self.board.reset()

    def search(self, team: int = None, iterations: int = 10000
               ) -> Tuple[int, int]:
        """
        An alias for search(Board, int, int). Please see Reach.search
        ```python
        >>> a = Board()
        >>> a.search()
        (9, 9)
        >>> b = Board()
        >>> search(b)
        (9, 9)
        ```
        """
        return search(self, team, iterations)

    def get(self, row: int, col: int) -> int:
        """
        Gets the piece (-1, 1 or 0) where -1 is a black stone (player 1)
            and 1 is a white stone, and 0 is empty of the position.
        ```python
        >>> a = Board()
        >>> a.play(9, 9)
        >>> a.get(9, 9)
        -1
        ```
        
        Args:  
        row: `int`
            The row of the piece starting from 0  

        col: `int`
            The column of the piece starting from 0  

        Returns:  
        piece: `int`
            The piece at the position (row, col) of the board.  

        Raises:  
        `IndexError`
            row or col are out of bounds.
        """
        if not 0 <= row < self.size or not 0 <= col < self.size:
            raise IndexError(f"Invalid coordinates (row, col): ({row}, {col}) "
                             f"exceeds dimentions of {self.size}x{self.size}.")
        return self.board.get(row, col)

    def play(self, row: int, col: int) -> None:
        """
        Places a piece at the location (row, col)  
        Args:  
        row `int`:
            The row of which to place the piece  
        col `int`:
            The column of which to place the piece  

        Raises:  
        `ValueError`
            row or col are out of bounds.
        """
        if not 0 <= row < self.size or not 0 <= col < self.size:
            raise ValueError(f"Invalid coordinates (row, col): ({row}, {col}) "
                             f"exceeds dimentions of {self.size}x{self.size}.")
        self.board.play(row, col)

    def undo(self) -> None:
        """Undoes the last move. May be called multiple times."""
        self.board.undo()


def search(ref_board: Board, team: int = None, iterations: int = 10000
           ) -> Tuple[int, int]:
    """
    Args:  
    ref_board: `Board`
        The board to be examined.  
    team: `int`
        The number of the piece that will be played this turn. First player is \
-1  
    iterations: `int`
        The amount of MCTS iterations to run.  
    
    Returns:  
    bestmove: `Tuple[int, int]`
        A 2-tuple of the position to place the next piece for best perceived \
outcome  

    Raises:
    `ValueError`
        when there is an object type mismatch

    ```python
    >>> a = Board()
    >>> a.search()
    (9, 9)
    >>> b = Board()
    >>> search(b)
    (9, 9)
    ```
    """
    if team is not None:
        print('You have chosen to manually overwrite the `team` parameter. '
              'It is recommended to allow this to handle this automatically.')
    else:
        team = (ref_board.move_count % 2) * 2 - 1

    if not isinstance(team, int):
        raise ValueError(f"team should be of {int} and not "
                         f"{type(team)}")
    if not isinstance(iterations, int):
        raise ValueError(f"iterations should be of {int} and not "
                         f"{type(iterations)}.")

    if isinstance(ref_board, Board):
        return search_(ref_board.board, team, iterations)
    elif isinstance(ref_board, Board_):
        return search_(ref_board, team, iterations)
    else:
        raise ValueError(f"ref_board should be of {Board} or {Board_} and not"
                         f" {type(ref_board)}.")
