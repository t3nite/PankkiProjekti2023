// Tarvittavien moduulien lataus
const accountinformation = require("../models/accountinformationmodel"); // Tilin tiedon malli
const card = require("../models/cardmodel"); // Kortin malli

// Kaikki tilitietojen hakemiseen liittyvät pyynnöt
const getAll = (req, res) => { 
    accountinformation.get(function(err,dbResult){
        if(err){
            res.json(err);
        }else{
            res.json(dbResult);
        }
    });
}

// Luottotilin tiedot hakemiseen liittyvä pyyntö
const getcredit = (req, res) => {
    accountinformation.getcredit(function(err,dbResult){
        if(err){
            res.json(err);
        }else{
            res.json(dbResult);
        }
    });
}

// Tietyn kortin tilitietojen hakemiseen liittyvä pyyntö ei tullut käyttöön lopussa
const getByCardNumber = (req, res) => {
    // Tarkistetaan, onko tarvittavat parametrit annettu
    if(!req.params.card_number || !req.params.page){
        return res.json({status:"error",message:"Please fill all fields."});
    }

    // Tarkistetaan, onko käyttäjällä pääsy korttiin
    card.getByiduser(req.iduser, function(err,dbResult){
        let hasAccessToCard = false;
        for(let i = 0; i < dbResult.length; i++){
            if(dbResult[i].card_number === req.params.card_number){
                hasAccessToCard = true;
            }
        }
        
        if(!hasAccessToCard){
            return res.json({status:"error",message:"User doesn't have access to this card"});
        }

        // Lasketaan sivut ja haetaan tilitiedot
        let perPageCount = 10; 
        let maxLogCount = 0;
        let currentPage = 0;
        let maxPage = 0;
        accountinformation.getMaxaccountinformationNum(req.params.card_number, function(err,dbResult){
            if(err){
                return res.json(err);
            }

            if(dbResult.length > 0){
                maxLogCount = dbResult[0].maxCount;
                maxPage = Math.ceil(maxLogCount/perPageCount); 

                if(req.params.page <= 0 || req.params.page > maxPage){
                    return res.json({status:"error",message:"Page number out of scope"});
                }

                currentPage = (req.params.page - 1) * perPageCount;
                
                accountinformation.getByCardNumber(req.params.card_number, currentPage, function(err,dbResult){
                    if(err){
                        return res.json(err);
                    }
    
                    if(dbResult.length > 0){
                        res.json({status:"success", maxPageAmount: maxPage, data:dbResult});
                    }else{
                        res.json({status:"error",message:"No information found for that card number"});
                    }
                })

            }else{
                return res.json({status:"error",message:"0 information found for that card number"});
            }
        })
    })
}

// Tietyn kortin kiinteän määrän tilitietojen hakemiseen liittyvä pyyntö ei tullut käyttöön lopussa
const getByCardNumberFixed = (req, res) => {
    card.getByiduser(req.iduser, function(err,dbResult){
        let hasAccessToCard = false;
        for(let i = 0; i < dbResult.length; i++){
            if(dbResult[i].card_number === req.params.card_number){
                hasAccessToCard = true;
            }
        }

        if(!hasAccessToCard){
            return res.json({status:"error",message:"User doesn't have access to this card"});
        }

        // Tarkistetaan, onko kortin numero annettu
        if(req.params.card_number){
            accountinformation.getByCardNumberFixed(req.params.card_number, function(err,dbResult){
                if(err){
                    res.json(err);
                }
    
                if(dbResult.length > 0){
                    res.json(dbResult);
                }else{
                    res.json({status:"error",message:"No information found for that card number"});
                }
    
            });
        }else{
            res.json({status:"error",message:"Please fill all fields."});
        }
    })
}

// Tietojen lisäämiseen liittyvä pyyntö
const add = (req, res) => {
    // Tarkistetaan, että kaikki tarvittavat kentät ovat täytetty
    if(1==1)
    {
        accountinformation.add(req.body,function(err, dbResult){
            if(err){
                    //return res.json(err);
                    res.send(err);        
            }else{
                return res.json({status:"success",message:"New information added succesfully!"});
            }
        });
    }else{
        return res.json({status:"error",message:"Please fill all fields."});
    }
}

// Luottorajan lisäämiseen liittyvä pyyntö
const addcredit = (req, res) => {
    if(1==1)
    {
        accountinformation.add(req.body,function(err, dbResult){
            if(err){
                    //return res.json(err);
                    res.send(err);        
            }else{
                return res.json({status:"success",message:"New information added succesfully!"});
            }
        });
    }else{
        return res.json({status:"error",message:"Please fill all fields."});
    }
}

// Tilitietojen poistamiseen liittyvä pyyntö
const deleteaccoutinformation = (req, res) => {
    accountinformation.delete(function(err, dbResult){
        if (err) {
            console.error("Virhe poistettaessa tilitietoja:", err);
            return res.status(500).json({ error: 'Virhe poistettaessa tilitietoja' });
        }
        console.log("Tilitieto poistettiin onnistuneesti.");
        return res.status(200).json({ message: 'Tilitieto poistettiin onnistuneesti' });
    });
};

// Viedään kaikki funktiot moduulina käytettäväksi muissa tiedostoissa
module.exports = {
    getAll,
    getcredit,
    getByCardNumber,
    getByCardNumberFixed,
    add,
    addcredit,
    deleteaccoutinformation
}
