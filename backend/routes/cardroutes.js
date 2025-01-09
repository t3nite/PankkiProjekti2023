// Moduulien lataus
var express = require('express'); // Express-kehys
var router = express.Router(); // Express-reituri
const db = require("../config/db"); // Tietokantayhteysmoduuli
const cardcontroller = require("../controllers/cardcontroller"); // Korttien ohjainmoduuli

// Reitit määritetty eri HTTP-pyyntötyypeille ja vastaavat käsittelijäfunktiot

// GET-pyyntö kaikkien korttien hakemiseksi
router.get('/getAllCards', cardcontroller.getAll);

// GET-pyyntö kortin numeron ja PIN-koodin hakemiseksi
router.get('/getcardnumberpin', cardcontroller.getcardnumberpin);

// POST-pyyntö uuden kortin lisäämiseksi
router.post("/addcard", cardcontroller.addCard);

// POST-pyyntö kortin lisäämiseksi tiliin
router.post("/cardtoaccount", cardcontroller.addCardToAccount);

// GET-pyyntö kortin tietojen hakemiseksi korttinumeron perusteella
router.get("/info/:card_number", cardcontroller.getCardAccountInfoByNumber);

// GET-pyyntö kortin tietojen hakemiseksi
router.get("/info", cardcontroller.getCardAccountInfo);

// GET-pyyntö yritysten lukumäärän hakemiseksi kortin numeron perusteella
router.get("/tries/:card_number", cardcontroller.getTries);

// GET-pyyntö kortin hakemiseksi korttinumeron perusteella
router.get('/:card_number', cardcontroller.getByCardNumber);

// GET-pyyntö korttien hakemiseksi käyttäjän ID:n perusteella
router.get('/', cardcontroller.getByiduser);

// PUT-pyyntö kortin lukituksen poistamiseksi
router.put("/unlock", cardcontroller.unlock);

// POST-pyyntö käyttäjän autentikointiin
router.post("/auth", cardcontroller.authenticate);

// DELETE-pyyntö kortin poistamiseksi
router.delete("/deletecard", cardcontroller.deleteCard);

module.exports = router; // Vienti, jotta reituri voidaan käyttää muualla sovelluksessa
