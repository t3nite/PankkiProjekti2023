// Tarvittavien moduulien lataus
const emailvalidator = require("email-validator");
const sanitizer = require("sanitizer");
const { json } = require("express/lib/response");
const account = require("../models/accountmodel");
const user = require("../models/usermodel");
const card = require("../models/cardmodel");
const res = require("express/lib/response");

// Kaikkien tilitietojen hakeminen
const getAll = (req, res) => {
    account.get(function(err, dbResult){
        if(err){
            return res.json(err);
        }else{
            return res.json(dbResult);
        }
    });
}
// Tilin saldon hakeminen tietyn tilinumeron perusteella
const getaccountbalance = (req, res) => {
    if(req.params.id)
    
    account.getaccountbalance(req.params.id,function(err, dbResult){
        if(err){
            return res.json(err);
        } else {
            return res.json(dbResult[0]);
        }
    });
}
// Luottorajan hakeminen tietyn tilinumeron perusteella
const getcreditlimit = (req, res) => {
    if(req.params.id)
    
    account.getcreditlimit(req.params.id,function(err, dbResult){
        if(err){
            return res.json(err);
        } else {
            return res.json(dbResult[0]);
        }
    });
}
// Tilin saldon päivittäminen
const updateBalance = (req, res) => {
    if(req.body.account_balance){

        account.updateBalance(req.body.account_balance, (err, dbResult) => {
            if(err){
                return res.json({status:"error", message:err});
            } else {
                console.log(dbResult);
                return res.json(dbResult);
            }
        });
    } else {
        return res.json({status:"error", message:"account_balance missing from request"});
    }
}
const updatecredit = (req, res) => { // Luottorajan päivittäminen
    if(req.body.credit_limit){
        account.updatecredit(req.body.credit_limit, (err, dbResult) => {
            if(err){
                return res.json({status:"error", message:err});
            } else {
                console.log(dbResult);
                return res.json(dbResult);
            }
        });
    } else {
        return res.json({status:"error", message:"credit_limit or idaccount missing from request"});
    }
}

getOwnedAccounts = (req, res) => { // hakee tilit käytttäjän perusteella ei tullut käyttöön
    account.getByiduser(req,function(err, dbResult){
        if(err){
            return res.json(err);
        }else{
            return res.json(dbResult);
        }
    });
}

const addAccount = (req, res) => { //lisää käyttäjä
    // if(req.iduser && req.body.name)
    if(1==1)
    {
        account.add(req.body,function(err, dbResult){
            if(err){
                    //return res.json(err);
                    res.send(err);        
            }else{
                return res.json({status:"success",message:"New account added succesfully!"});
            }
        });
    }else{
        return res.json({status:"error",message:"Please fill all fields."});
    }
}

const addUserToAccount = (req, res) => { //käyttäjän ja tilin yhdistäminen
    if((req.body.iduser) && req.body.idaccount)
   // if (1==1)
      {  
        account.addUserToAccount(req.body.iduser, req.body.idaccount, function(err, dbResult){
if(err){
    res.send(err);
       
        }else{
            return res.json({status:"success",message:"New account and user added succesfully!"});
        }  
    });
}else{
    return res.json({status:"error",message:"Please fill all fields."});
    }
}
const getConnectedUsers = (req, res) => {// hakee tilit joilla on käyttäjä ei tullut käyttöön
    if(req.params.id){
        account.getOwnerById(req.iduser, req.params.id, function(err, dbResult){
            if(err){
                return res.json({status:"error",message:err});
            }
            let hasAccessToAccount = false;
            let accountId = null;
            for(let i=0;i<dbResult.length;i++){
                if(dbResult[i].account_ID === Number(req.params.id) && dbResult[i].owner === req.iduser){
                    hasAccessToAccount = true;
                    accountId = dbResult[i].account_ID;
                }
            }
            if(!hasAccessToAccount){
                return res.json({status:"error",message:"You are not the owner of this account."});
            }
            account.getConnectedUsers(accountId, req.iduser, function(err, dbResult){
                if(err){
                    return res.json({status:"error",message:err});
                }
                if(dbResult.length > 0){
                    return res.json({status:"success",message:dbResult});
                }else{
                    return res.json({status:"error",message:"No users found."});
                }
            });
        });
    }else{
        return res.json({status:"error",message:"Please fill all fields."});
    }
}


const disconnectUser = (req, res) => { // poistaa käyttäjän tilistä ei tullut käyttöön
    if(req.body.account && req.body.user){
        account.getOwnerById(req.iduser, req.body.account, function(err, dbResult){
            if(err){
                return res.json({status:"error",message:err});
            }
            let hasAccessToAccount = false;
            for(let i=0;i<dbResult.length;i++){
                if(dbResult[i].idaccount.toString() === req.body.account.toString() && dbResult[i].owner.toString() === req.iduser){
                    hasAccessToAccount = true;
                }
            }
            if(!hasAccessToAccount){
                return res.json({status:"error",message:"You are not the owner of this account."});
            }
            account.deleteUserFromAccount(req.body.account, req.body.user, function(err, dbResult){
                if(err){
                    return res.json({status:"error",message:err});
                }
                if(dbResult.affectedRows > 0){
                    return res.json({status:"success",message:"Successfully removed user from account."});
                }else{
                    return res.json({status:"error",message:"An unknown error has occurred."});
                }
            });
        });
    }else{
        return res.json({status:"error",message:"Please fill all fields."});
    }
}

const deleteAccount = (req, res) => { // tilin poisto
    if(req.body.idaccount){
        account.delete(req.idaccount,function(err, dbResult){
            if (err) {
                console.error("Virhe poistettaessa tiliä:", err);
                return res.status(500).json({ error: 'Virhe poistettaessa tiliä' });
            }
            console.log("Tili poistettiin onnistuneesti.");
            return res.status(200).json({ message: 'Tili poistettiin onnistuneesti' });
        });
    }
};
//vienti
module.exports = {
    getAll,
    getaccountbalance,
    getcreditlimit,
    updateBalance,
    updatecredit,
    getOwnedAccounts,
    getConnectedUsers,
    addAccount,
    addUserToAccount,
    disconnectUser,
    deleteAccount,

}