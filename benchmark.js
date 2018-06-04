const exec = require('child_process').exec;

async function countTimeAsSeconds(execFunction) {
    const startTime = process.hrtime();
    await execFunction();
    const endTime = process.hrtime(startTime);

    const [seconds, nanos] = endTime;
    const time = Number(`${seconds}.${nanos}`);
    return Number(time.toFixed(2));
}

function sleep(ms) {
    return new Promise(resolve => setTimeout(resolve, ms));
}

async function sh(cmd, log = false) {
    return new Promise(function(resolve, reject) {
        exec(cmd, (err, stdout, stderr) => {
            if (err) {
                reject(err);
                console.log(err);
            } else {
                resolve({stdout, stderr});
                if (log && stdout.trim()) {
                    console.log(stdout);
                }
            }
        });
    });
}

function generateCommand(slaveThreads, boardSize) {
    return `mpiexec.exe -n ${slaveThreads +
        1} ParallelColumnsLauncher ${boardSize} 1000 random false false`;
}

const MAX_NUMBER_OF_THREADS = 12;

async function benchmark(boardSize) {
    const results = {};

    for (let i = 1; i <= MAX_NUMBER_OF_THREADS; i++) {
        try {
            const time = await countTimeAsSeconds(async () => {
                await sh(generateCommand(i, boardSize), false);
            });
            results[i] = time;
            console.log(`${boardSize}x${boardSize} | ${i} Slaves = ${time}s`);
            await sleep(100);
        } catch (e) {
            console.error(e);
        }
    }
    console.log(`________ ${boardSize}x${boardSize} RESULTS ________`);
    printResults(results);
    console.log(`///////////////////////////\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\`);
}

function printResults(results) {
    Object.keys(results).forEach(key => {
        console.log(String(results[key]).replace('.', ','));
    })
}

benchmark(2048);
