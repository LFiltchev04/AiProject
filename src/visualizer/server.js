//the whlole idea of this app is to read trought persisted playtesting data or live test information, but while vizualizing a prticular AI`s internal map

let express = require("express");
let app = express();

app.use(express.json()); 

app.post("/addLog",(req,res)=>{
    
    let data = req.body;
    

    res.sendStatus(300);

})




app.listen(3005,()=>{
    console.log("Viz server is up");
})
