var socket = io();
let nostalgist;
let screenSizeL;
let screenSizeS;
let windowInnerWidth;
let windowInnerHeight;
let gamesPerRow;
let nbrRows;
let gamesList;

window.addEventListener("load", (event) => {
    if (/Android|webOS|iPhone|iPad|iPod|BlackBerry|IEMobile|Opera Mini/i.test(navigator.userAgent)) {
        // true for mobile device
        console.log("mobile device");
        screenSizeS = true;
        screenSizeL = false;
        gamesPerRow = 8;
    } else {
        // false for not mobile device
        console.log("not mobile device");
        screenSizeS = false;
        screenSizeL = true;
        gamesPerRow = 9;
        windowInnerWidth = document.documentElement.clientWidth;
        windowInnerHeight = document.documentElement.clientHeight;
        console.log(`Client width: ${windowInnerWidth}, Client height: ${windowInnerHeight}`)
    }

    fetchGamesData();
});

function removeGames() {
    var elements = document.querySelectorAll('.games-container');

    if (elements.length > 0) {
        console.log("Exists!");

        // Remove each element with the class 'games-container'
        elements.forEach(element => element.remove());
    } else {
        console.log("Doesn't exist!");
    }
}

async function updateUI() {
    windowInnerWidth = document.documentElement.clientWidth;
    windowInnerHeight = document.documentElement.clientHeight;

    console.log(`Client width: ${windowInnerWidth}, Client height: ${windowInnerHeight}`);
    removeGames();

    if (typeof gamesList !== 'undefined') {
        const imageSize = windowInnerWidth / gamesPerRow;

        // Create a container div to hold all games
        const gamesContainer = document.createElement('div');
        gamesContainer.className = 'games-container';
        document.body.appendChild(gamesContainer);

        await Promise.all(gamesList.map(async (game, index) => {
            const gameDiv = document.createElement('div');
            gameDiv.className = 'game';

            const imageElement = new Image();
            imageElement.src = game.gameCover;
            imageElement.alt = `${game.gameName} Cover`;

            // Wrap image loading in a promise
            const imageLoadPromise = new Promise((resolve, reject) => {
                imageElement.onload = () => {
                    const canvas = document.createElement("canvas");
                    const ctx = canvas.getContext("2d");
                    canvas.width = imageSize;
                    canvas.height = 1.5 * imageSize;
                    const radius = 20; // Adjust the radius as needed
                    ctx.beginPath();
                    ctx.moveTo(radius, 0);
                    ctx.arcTo(canvas.width, 0, canvas.width, canvas.height, radius);
                    ctx.arcTo(canvas.width, canvas.height, 0, canvas.height, radius);
                    ctx.arcTo(0, canvas.height, 0, 0, radius);
                    ctx.arcTo(0, 0, canvas.width, 0, radius);
                    ctx.closePath();

                    // Draw the image inside the rounded rectangle
                    ctx.clip(); // Clip to the rounded rectangle
                    ctx.drawImage(imageElement, 0, 0, canvas.width, canvas.height);
                    ctx.fillStyle = 'white';
                    ctx.font = '18px Arial'; // Set font size and family
                    // ctx.textAlign = 'center'; // Center horizontally
                    // ctx.textBaseline = 'bottom';

                    const mouseOnTop = () => {
                        ctx.clearRect(0, 0, canvas.width, canvas.height); // Clear the canvas
                        ctx.drawImage(imageElement, 0, 0, canvas.width, canvas.height);
                        ctx.fillText(game.gameName, 20, 20);
                    };

                    // Function to clear text on mouseleave
                    const mouseLeaves = () => {
                        ctx.clearRect(0, 0, canvas.width, canvas.height); // Clear the canvas
                        ctx.drawImage(imageElement, 0, 0, canvas.width, canvas.height);
                    };

                    const canvasClickHandler = (gameName) => {
                        console.log(`Canvas clicked for game: ${gameName}`);
                        launchGame(gameName);
                        // Call your custom function or perform any desired action with gameName
                    };

                    // Handle mouseenter event
                    gameDiv.addEventListener('mouseenter', mouseOnTop);

                    // Handle mouseleave event
                    gameDiv.addEventListener('mouseleave', mouseLeaves);

                    // Handle canvas click event
                    canvas.addEventListener('click', () => canvasClickHandler(game.gameName));

                    gameDiv.appendChild(canvas);
                    gamesContainer.appendChild(gameDiv);
                    resolve();
                };

                imageElement.onerror = () => {
                    console.error(`Failed to load image for ${game.gameName}`);
                    reject();
                };
            });

            if ((index + 1) % nbrRows !== 0) {
                gameDiv.style.marginRight = 80 / gamesPerRow + 'px';
            }
            gameDiv.style.marginBottom = 80 / gamesPerRow + 'px';
            gameDiv.style.position = 'relative';  // Set position to relative
            gameDiv.style.transition = 'filter 0.3s';
            gameDiv.style.filter = 'brightness(100%)';  // Initial brightness

            // Add mouseover event for shading effect
            gameDiv.addEventListener('mouseover', () => {
                gameDiv.style.filter = 'brightness(50%)';  // Adjust brightness on mouseover
            });

            // Add mouseout event for removing shading effect
            gameDiv.addEventListener('mouseout', () => {
                gameDiv.style.filter = 'brightness(100%)';  // Adjust brightness on mouseout
            });
            await imageLoadPromise;
        }));

        gamesContainer.style.flexWrap = 'wrap';
        gamesContainer.style.display = 'flex';
        gamesContainer.style.justifyContent = 'center';
    }
}
class Game {
    constructor(name, url, description, cover) {
        this.gameName = name;
        this.gameUrl = url;
        this.gameDescription = description;
        this.gameCover = cover;
    }
}

const githubApiUrl = 'https://api.github.com/repos/retrobrews/nes-games/contents';

async function fetchGamesData() {
    try {
        const response = await fetch(githubApiUrl);
        if (!response.ok) {
            throw new Error(`Failed to fetch data: ${response.status}`);
        }

        const responseData = await response.json();

        // Filter for files only
        const filesData = responseData.filter(item => item.type === 'file');

        // Group files by game name (prefix before the file extension)
        const gamesMap = new Map();

        // Function to handle a single file
        const handleFile = async (file) => {
            // Exclude these files
            if (file.name.toLowerCase() === 'readme.md' || file.name.toLowerCase() === 'gamelist.xml') {
                return;
            }

            const [gameName, extension] = file.name.split('.');
            const game = gamesMap.get(gameName) || new Game(gameName);

            if (extension === 'nes') {
                game.gameUrl = file.download_url;
            } else if (extension === 'txt') {
                try {
                    // Fetch the content of the text file
                    const textFile = await fetch(file.download_url);

                    if (textFile.ok) {
                        const textContent = await textFile.text();
                        game.gameDescription = textContent;
                    } else {
                        console.error(`Failed to fetch text file for ${gameName}`);
                    }
                } catch (error) {
                    console.error(`Error fetching text file for ${gameName}:`, error);
                }
                // console.log("File: ", file);
            } else if (extension === 'png') {
                game.gameCover = file.download_url;
            }

            gamesMap.set(gameName, game);
        };

        // Process each file, logging failed requests
        filesData.forEach(async (file) => {
            try {
                await handleFile(file);
            } catch (error) {
                console.error(`Failed to handle file ${file.name}: ${error.message}`);
            }
        });

        // Convert Map values to an array of Game instances
        gamesList = Array.from(gamesMap.values());

        // Custom sorting function
        const customSort = (a, b) => {
            const nameA = a.gameName.toLowerCase();
            const nameB = b.gameName.toLowerCase();

            // Custom logic for sorting
            if (/^\d/.test(nameA) && !/^\d/.test(nameB)) {
                return 1; // Numbers come last
            } else if (!/^\d/.test(nameA) && /^\d/.test(nameB)) {
                return -1; // Numbers come last
            } else {
                return nameA.localeCompare(nameB); // Default sorting
            }
        };
        gamesList.sort(customSort);

        await updateUI();
        return gamesList;
    } catch (error) {
        console.error('Error fetching or parsing data:', error);
    }
}

window.addEventListener('keydown', function (event) {
    // Check if the key pressed is 'C' and Ctrl key is also pressed
    if (event.key === 'c' && event.ctrlKey) {
        // Perform actions when Ctrl+C is pressed
        console.log('Ctrl+C pressed!');
        pressExit();
    }
});

async function launchGame(gametoRun) {
    try {
        // launch Flappy Bird with an NES emulator!
        nostalgist = await Nostalgist.nes(gametoRun + ".nes");

        // Send a message to the server indicating that the game is launched
        socket.emit('gameLaunched', { gameName: gametoRun });

    } catch (error) {
        console.error('Error launching the game:', error);
    }
}

socket.on('connect', function () {
    socket.emit('my event', { data: 'I\'m connected!' });
});
socket.on('error', (error) => {
    console.error('WebSocket error:', error);
});

socket.on('close', (event) => {
    console.log('WebSocket closed:', event);
});


window.addEventListener('unload', pressExit());
async function pressExit() {
    console.log("Exit");
    try {
        if (nostalgist != undefined) {
            await nostalgist.exit()
        }
        socket.emit('gameClose', { gameName: "exit" });
    } catch (error) {
        console.error(`Error exiting: ${error}`)
    }
}

socket.on('command_input', function (data) {
    // Call the asynchronous function from within the callback
    handleCommandInput(data).catch(error => {
        console.error('Error handling command input:', error);
    });
});

async function handleCommandInput(data) {
    if (nostalgist != undefined) {
        console.log("Received data from server:", data);
        console.log(data.command);
        // up,down,left,right,select,start,a,b
        switch (data.command) {
            case "start":
                await nostalgist.press('start');
                break;
            case "stop":
                pressExit();
                break;
            case "pause":
                await nostalgist.pause()//Add pause/resume logic
                break;
            default:
                if (data.command !== "cmd"){
                    await nostalgist.press(data.command);
                }
                break;

        }
    }
}