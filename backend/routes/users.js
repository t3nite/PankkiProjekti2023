// Moduulin lataus
var express = require('express');
var router = express.Router(); // Luo Express-reituri-objektin
var db = require("../config/db"); // Tietokantayhteyden muodostamiseen tarvittava moduuli
const usercontroller = require("../controllers/usercontroller"); // Käyttäjäohjaimen moduuli

// Reitit määritetty eri HTTP-pyyntötyypeille ja vastaavat käsittelijäfunktiot
router.get("/info", usercontroller.userInfo); // Palauttaa käyttäjätietoja annetun ID:n perusteella
router.get('/user', usercontroller.getAll); // Palauttaa kaikki käyttäjätietueet
router.get('/:id', usercontroller.getByiduser); // Palauttaa käyttäjätiedot annetun ID:n perusteella
router.post("/login", usercontroller.userLogin); // Käyttäjän kirjautuminen
router.post("/register", usercontroller.userRegister); // Käyttäjän luominen
router.put("/avatar", usercontroller.updateAvatar); // Päivittää käyttäjän avatarin
router.delete("/deleteuser", usercontroller.deleteuser); // Poistaa käyttäjän

module.exports = router; // Vienti, jotta tätä reittiä voidaan käyttää muualla sovelluksessa
