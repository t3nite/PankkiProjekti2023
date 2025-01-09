// Tarvittavien moduulien lataus
var express = require('express');
var router = express.Router();
const db = require("../config/db"); // Tietokantayhteys
const accountinformationcontroller = require("../controllers/accountinformationcontroller.js"); // Tilitietojen ohjain

// Reitit ja niihin liittyvät ohjaimet

// Tilitietojen hakeminen
router.get("/info",accountinformationcontroller.getAll);

// Luottotilin tiedot hakeminen
router.get("/infocredit",accountinformationcontroller.getcredit);

// Tietyn kortin tilitietojen hakeminen
router.get("/getByCardNumber/:card_number/:page",accountinformationcontroller.getByCardNumber);

// Tietyn kortin kiinteän määrän tilitietojen hakeminen
router.get("/getByCardNumber/:card_number",accountinformationcontroller.getByCardNumberFixed);

// Uusien tilitietojen luominen
router.post("/create",accountinformationcontroller.add);

// Uuden luottotiedon luominen
router.post("/createcredit",accountinformationcontroller.add);

// Tilitietojen poistaminen
router.delete("/delete",accountinformationcontroller.deleteaccoutinformation);

// Viedään reitit muissa tiedostoissa käytettäväksi
module.exports = router;
