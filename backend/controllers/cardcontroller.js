
// Tarvittavien moduulien lataus
const card = require("../models/cardmodel");
const account = require("../models/accountmodel");
const bcrypt = require("bcrypt");

// Funktio hakee kaikki kortit tietokannasta
const getAll = (req, res) => {
    card.get(function(err,dbResult){
        if(err){
            return res.json({status:"error",message:err});
        }else{
            res.json({status:"success",message:dbResult});
        }
    });
}
// Funktio hakee kortin numeron ja PIN-koodin tietokannasta tietyn kortin id:n perusteella
const getcardnumberpin = (req, res) => {
    card.getcardnumberpin(function(err,dbResult){
        if(err){
            return res.json({status:"error",message:err});
        }else{
            res.json(dbResult[0]);
        }
    });
}
// Funktio lisää uuden kortin tietokantaan
const addCard = (req, res) => {
    //if(req.body.pin && req.body.card_number && req.body.card_type){
// Tarkistetaan pyynnön tiedot ennen lisäämistä        
if(1==1)
{
    
        card.addCard(req.body,function(err, dbResult){
            if(err){
                    //return res.json(err);
                    res.send(err);        
            }else{
                return res.json({status:"success",message:"New card added succesfully!"});
            }
        });
    }else{
        return res.json({status:"error",message:"Please fill all fields."});
    }
}




const getByCardNumber = (req, res) => { //kortti haku numeron perusteella ei tullut käyttöön

    if(req.params.card_number){
        card.getByiduser(req.iduser, (err, dbResult) =>{

            if(err){
                return res.json({status:"error",message:err});
            }

            let hasAccessToCard = false;
            for(let i = 0; i < dbResult.length; i++){
                if(dbResult[i].card_number.toString() === req.params.card_number){
                    hasAccessToCard = true;
                }
            }

            if(!hasAccessToCard){
                return res.json({status:"error",message:"User doesn't have this card"});
            }

            card.getByNumber(req.params.card_number, function(err,dbResult){
                if(err){
                    return res.json({status:"error",message:err});
                }
    
                if(dbResult.length > 0){
                    return res.json(dbResult);
                }else{
                    return res.json({status:"error",message:"Card not found"})
                }
            });
        });
    }else{
        return res.json({status:"error",message:"Please fill all fields"});
    }
}

const getByiduser = (req, res) => { //korttien haku käyttäjän perusteella ei tullut käyttöön
    card.getByiduser(req.iduser, function(err,dbResult){

        if(err){
            return res.json({status:"error",message:err})
        }

        if(dbResult.length > 0){
            res.json(dbResult);
        }else{
            res.json({status:"error",message:"No cards found for this user"})
        }

    });
}

const getCardAccountInfo = (req, res) => { //kortin tilin tiedot ei tullut käyttöön
    card.getCardAccountInfo(req.iduser, function(err, dbResult){
        if(err){
            return res.json({status:"error",message:err});
        }
        if(dbResult.length > 0){
            return res.json(dbResult);
        }else{
            return res.json({status:"error",message:"No cards found for this user"});
        }
    });
}

const getCardAccountInfoByNumber = (req, res) => { //kortin tilin tiedot numeron perusteella haku
    if(req.params.card_number){
        card.getByiduser(req.iduser, (err, dbResult) =>{

            if(err){
                return res.json({status:"error",message:err});
            }

            let hasAccessToCard = false;
        
            for(let i = 0; i < dbResult.length; i++){
                if(dbResult[i].card_number === req.params.card_number){
                    hasAccessToCard = true;
                }
            }

            if(!hasAccessToCard){
                return res.json({status:"error",message:"User doesn't have this card"});
            }

            card.getCardAccountInfoByNumber(req.params.card_number, function(err, dbResult){
                if(err){
                    return res.json({status:"error",message:err});
                }
                if(dbResult.length > 0){
                    return res.json(dbResult[0]);
                }else{
                    return res.json({status:"error",message:"No cards found for this user"});
                }
            });
        });
    }
}

const unlock = (req, res) => { //kortin lukituksen avaus ei tullut käyttöön
    if(req.body.card_number){

        card.getByiduser(req.iduser, (err, dbResult) =>{

            if(err){
                return res.json({status:"error",message:err});
            }

            let hasAccessToCard = false;
        
            for(let i = 0; i < dbResult.length; i++){
                if(dbResult[i].card_number === req.body.card_number){
                    hasAccessToCard = true;
                }
            }

            if(!hasAccessToCard){
                return res.json({status:"error",message:"User doesn't have this card"});
            }

    


            card.updateTries(0, req.body.card_number, (err, dbResult) =>{

                if(err){
                    return res.json({status:"error",message:"Error on resetting card's tries"})
                }

                if(dbResult.affectedRows > 0){
                    console.log("Unlocking card, resetting tries");
                }else{
                    console.log("Error on resetting tries");
                }
            });

            card.updateActiveStatus(1, req.body.card_number, function(err, dbResult){

                if(err){
                    return res.json({status:"error",message:err})
                }
                
                if(dbResult.affectedRows > 0){
                    res.json({status:"success",message:"Successfully modified card's status"});
                }else{
                    res.json({status:"error",message:"Card not found"})
                }
                
            });
        })
        
    }else{
        res.json({status:"error",message:"Please fill all fields"});
    }
}


const addCardToAccount = (req, res) => { //vanha kortin lisäys tiliin ei tullut käyttöön
    if(req.userId && req.body.accountId && req.body.card_type && req.body.pin){
        const pinRegex = /\D/;
        const pinInput = req.body.pin.toString();
        let validatePin = false;
        if(!pinInput.match(pinRegex) && pinInput.length === 4){
            validatePin = true;
        }
        if(!validatePin){
            return res.json({status:"error",message:"Pin must be exactly 4 digits long."});
        }
        let ctype = null;
        if(req.body.card_type === "Debit"){
            ctype = 0;
        }else if(req.body.card_type === "Credit"){
            ctype = 1;
        }else{
            return res.json({status:"error",message:"Invalid card type."});
        }
        account.getByUserId(req, function(err, dbResult){
            if(err){
                return res.json({status:"error",message:err});
            }else{
                let hasAccessToAccount = false;
                for(let i=0;i<dbResult.length;i++){
                    if(dbResult[i].account_ID === req.body.idaccount){
                        hasAccessToAccount = true;
                    }
                }
                if(!hasAccessToAccount){
                    return res.json({status:"error",message:"User does not have access to this account."});
                }
                card.connectCard(req.body.accountId, req.userId, ctype, req.body.pin, function(err, dbResult){
                    if(err){
                        return res.json({status:"error",message:err});
                    }else if(dbResult.affectedRows === 0){
                        return res.json({status:"error",message:"No cards available."});
                    }
                    else{
                        return res.json({status:"success",message:"Successfully added card to selected account."});
                    }
                });
            }
        });
    }else{
        return res.json({status:"error",message:"Please fill all fields."});
    }
}


const disconnectCard = (req, res) => { //vanha kortin deletointi ei tullut käyttöön
    if(req.body.card_number){
        card.getByiduser(req.iduser, function(err, dbResult){
            if(err){
                return res.json({status:"error",message:err});
            }
            let hasAccessToCard = false;
            for(let i=0;i<dbResult.length;i++){
                if(dbResult[i].card_number === req.body.card_number){
                    hasAccessToCard = true;
                }
            }
            if(!hasAccessToCard){
                return res.json({status:"error",message:"User does not have access to this card."});
            }
            card.disconnectCard(req.body.card_number, function(err, dbResult){
                if(err){
                    return res.json({status:"error",message:err});
                }
                if(dbResult.affectedRows > 0){
                    return res.json({status:"success",message:"Successfully deleted card."});
                }else{
                    return res.json({status:"error",message:"Unknown error has occurred."});
                }
            });
        });
    }else{
        return res.json({status:"error",message:"Please fill all fields."});
    }
}

const getTries = (req, res) => { // yritysten ohjain ei tullut käyttöön
    if(req.params.card_number){
        card.getTries(req.params.card_number, function(err, dbResult){
            if(dbResult.length > 0){
                return res.json({tries:dbResult[0].tries});
            }else{
                return res.json({status:"error",message:"Card was not found."});
            }
        });
    }else{
        return res.json({status:"error",message:"Please fill all fields."});
    }
}

const authenticate = (req, res) => { //kortin avaus ja yritysten resetointi 

    let iduser = null;

    if(req.body.card_number && req.body.pin){
        card.getByNumber(req.body.card_number, function(err, dbResult){
            if(err){
                return res.json({status:"error",message:err})
            }

    
            if(dbResult.length > 0){

                iduser = dbResult[0].iduser;
                if(dbResult[0].active === 0){
                    return res.json({status:"error",message:"Card is locked!"});
                }
                
                bcrypt.compare(req.body.pin, dbResult[0].pin.toString(), (err, match)=>{
                    if(err){
                        return res.json({status:"error",message:err})
                    }

                    if(match){
                    
                        console.log("Created token:",token);
                        card.updateTries(0, req.body.card_number, (err, dbResult) =>{
                            if(err){
                                console.log("Error on resetting card tries.");
                            }
        
                            if(dbResult.affectedRows > 0){
                                console.log("Unlocking card, resetting tries");
                            }else{
                                console.log("Error on resetting tries");
                            }
                        });
                        //return res.json({status:"success",message:"Successfully logged in.",token:token});//
                    }else{
                        console.log("Invalid pin code or card number!");

                        let tries = dbResult[0].tries + 1;
                        
                        card.updateTries(tries, req.body.card_number, (err, dbResult) =>{
                            if(err){
                                console.log(err);
                            }

                            if(dbResult.affectedRows > 0){
                                console.log("Incremented tries");
                            }else{
                                console.log("Error on incrimenting tries");
                            }
                        })

                      
                        if(tries === 3 && dbResult[0].active === 1){
                            card.updateActiveStatus(0, req.body.card_number, (err, dbResult) =>{
                                if(err){
                                    console.log(err);
                                }

                                if(dbResult.affectedRows > 0){
                                    console.log("Card is deactivated");
                                }else{
                                    console.log("Error on deactivating card");
                                }

                                return res.json({status:"error",message:"Card deactivated!"});
                            })
                        }else{
                            return res.json({status:"error",message:"Invalid pin code!", tries:tries});
                        }
                    }
                });

            }else{
                res.json({status:"error",message:"Card was not found"});
            }
            
        });
    }else{
        res.json({status:"error",message:"Please fill all fields"})
    }
    
}

const deleteCard = (req, res) => { // kortin deletointi
    if(req.body.idcard){
        card.delete(req.body.idcard,function(err, dbResult){
            if (err) {
                console.error("Virhe poistettaessa kortteja:", err);
                return res.status(500).json({ error: 'Virhe poistettaessa kortteja' });
            }
            console.log("Kortti poistettiin onnistuneesti.");
            return res.status(200).json({ message: 'Kortti poistettiin onnistuneesti' });
        });
    }
};

//vienti
module.exports = {
    getAll,
    getcardnumberpin,
    addCard,
    addCardToAccount,
    getByCardNumber,
    getByiduser,
    getCardAccountInfo,
    getCardAccountInfoByNumber,
    getTries,
    unlock,
    authenticate,
    deleteCard,
    
}