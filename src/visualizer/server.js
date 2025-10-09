//the whlole idea of this app is to read trought persisted playtesting data or live test information, but while vizualizing a prticular AI`s internal map

let express = require("express");
let app = express();

app.use(express.json()); 


//this is the data strctures to hold the logs, no networking yet
//there is a fast read option that holds a direct reference to the array to avoid has computation, when the frontend queries in stream mode it sends an index of the last element if it got overriden by accident



const logByID = new Map();



async function fastWrites(logElem){
    
    logElem.seqID++;

    if(streamArray.length == 7){
        streamArray.push(logElem);
        streamArray.shift();
    }

    var tempArrPtr = logByID.get(logElem.seqID);
    tempArrPtr.push(logElem);

}

async function fastFetch(aiID,seqIdx){
    tempArrRef = logByID.get(aiID);
    return tempArrRef.slice(seqIdx,tempArrRef.length-1);
}


async function newEntry(id, entry){
    


    if(logByID.has(id)){

        if(id == streamLogID){

        }

        logByID.get(id).push(entry);
        return;
    }

    logByID.set(id,[]);
}

function fetchEntries(id){
    return logByID.get(id);
}




app.get("/checkIsOn",(res)=>{
    res.sendStatus(300);
})
  

app.post("/addLog",(req,res)=>{
    let data = req.body;

    newEntry(data.id,data.body);
    res.sendStatus(300);
})

app.post("/fetchFrontend",(req,res)=>{
    let data = req.body;

    if(data.id == null){
        res.sendStatus(404);
    }

    if(data.id == streamLogID){

    }

    res.body = JSON.stringify({positions:fetchEntries(data.id)});
})




app.listen(3005,()=>{
    console.log("Viz server is up");
})
