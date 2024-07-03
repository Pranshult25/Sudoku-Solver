document.addEventListener("DOMContentLoaded", function () {
    var arr = [[], [], [], [], [], [], [], [], []];

    for (var i = 0; i < 9; i++) {
        for (var j = 0; j < 9; j++) {
            arr[i][j] = document.getElementById(i * 9 + j);
        }
    }

    var board = [
        [0, 0, 0, 0, 0, 0, 0, 0, 0],
        [0, 0, 0, 0, 0, 0, 0, 0, 0],
        [0, 0, 0, 0, 0, 0, 0, 0, 0],
        [0, 0, 0, 0, 0, 0, 0, 0, 0],
        [0, 0, 0, 0, 0, 0, 0, 0, 0],
        [0, 0, 0, 0, 0, 0, 0, 0, 0],
        [0, 0, 0, 0, 0, 0, 0, 0, 0],
        [0, 0, 0, 0, 0, 0, 0, 0, 0],
        [0, 0, 0, 0, 0, 0, 0, 0, 0]
    ];

    function FillBoard(board) {
        for (var i = 0; i < 9; i++) {
            for (var j = 0; j < 9; j++) {
                if (board[i][j] != 0) {
                    arr[i][j].innerText = board[i][j];
                } else {
                    arr[i][j].innerText = '';
                }
            }
        }
    }

    let GetPuzzle = document.getElementById('GetPuzzle');
    let SolvePuzzle = document.getElementById('SolvePuzzle');

    GetPuzzle.onclick = function () {
        var xhrRequest = new XMLHttpRequest();
        xhrRequest.onload = function () {
            var response = JSON.parse(xhrRequest.response);
            console.log(response);
            board = response.board;
            FillBoard(board);
        };
        xhrRequest.open('get', 'https://sugoku.onrender.com/board?difficulty=easy');
        xhrRequest.send();
    };

    SolvePuzzle.onclick = function () {
        if (solveSudoku(board, 9)) {
            FillBoard(board);
        } else {
            alert('No solution found!');
        }
    };

    function isValid(puzzle, row, col, value, n) {
        for (let c = 0; c < n; c++) {
            if (puzzle[row][c] === value) return false;
        }
        for (let r = 0; r < n; r++) {
            if (puzzle[r][col] === value) return false;
        }
        let startRow = Math.floor(row / 3) * 3;
        let startCol = Math.floor(col / 3) * 3;
        for (let r = startRow; r < startRow + 3; r++) {
            for (let c = startCol; c < startCol + 3; c++) {
                if (puzzle[r][c] === value) return false;
            }
        }
        return true;
    }

    function hasEmptyCell(puzzle, n) {
        for (let r = 0; r < n; r++) {
            for (let c = 0; c < n; c++) {
                if (puzzle[r][c] === 0) return true;
            }
        }
        return false;
    }

    function solveSudoku(puzzle, n) {
        let row = -1, col = -1;
        let isEmpty = true;
        for (let i = 0; i < n; i++) {
            for (let j = 0; j < n; j++) {
                if (puzzle[i][j] === 0) {
                    row = i;
                    col = j;
                    isEmpty = false;
                    break;
                }
            }
            if (!isEmpty) break;
        }
        if (isEmpty) return true;

        for (let value = 1; value <= n; value++) {
            if (isValid(puzzle, row, col, value, n)) {
                puzzle[row][col] = value;
                if (solveSudoku(puzzle, n)) return true;
                puzzle[row][col] = 0;
            }
        }
        return false;
    }
});
