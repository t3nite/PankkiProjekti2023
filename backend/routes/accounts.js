// Tarvittavien moduulien lataus
var express = require('express'); // Express-moduuli reittien määrittelyyn
var router = express.Router(); // Express-reittien määrittelyyn tarkoitettu router-olio
const db = require("../config/db"); // Tietokantayhteyden konfiguraatiomoduuli
const accountController = require("../controllers/accountController"); // Tili-ohjaimen moduuli, joka sisältää toiminnot tiliin liittyen


// Reitit määritellään tässä

// Hakee kaikki tilit tietokannasta
router.get('/accountall',accountController.getAll);

// Hakee tilin saldon tietyn tilin ID:n perusteella
router.get("/getaccountbalance/:id",accountController.getaccountbalance);

// Hakee luottorajan tietyn tilin ID:n perusteella
router.get("/getcreditlimit/:id",accountController.getcreditlimit)

// Päivittää tilin saldoa
router.put("/update", accountController.updateBalance);

// Päivittää tilin luottorajaa
router.put("/updatecredit", accountController.updatecredit);

// Hakee käyttäjän omistamat tilit
router.get("/ownedaccounts",accountController.getOwnedAccounts);

// Hakee tietyn tilin käyttäjät tietyn tilin ID:n perusteella
router.get("/:id/users", accountController.getConnectedUsers);

// Lisää käyttäjän tilille
router.post("/addusertoaccount", accountController.addUserToAccount);

// Lisää uuden tilin
router.post("/addaccount",accountController.addAccount);

// Poistaa käyttäjän tililtä
router.delete("/user",  accountController.disconnectUser);

// Poistaa tilin
router.delete("/deleteacc",  accountController.deleteAccount);


// Viedään router-olio moduulina käytettäväksi muissa tiedostoissa
module.exports = router;
