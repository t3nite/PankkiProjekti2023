var express = require('express');
var path = require('path');
var cookieParser = require('cookie-parser');
var logger = require('morgan');
// Reittien moduulit
var indexRouter = require('./routes/index');
const usersRouter = require('./routes/users.js');
const accountsRouter = require('./routes/accounts.js');
const cardroutesRouter = require('./routes/cardroutes.js');
const accountinformationRouter = require ('./routes/accountinformation.js');
const jwt = require('jsonwebtoken');
var loginRouter = require('./routes/login.js');


var app = express();

// Middleware-komponentit
app.use(logger('dev'));
app.use(express.json());
app.use(express.urlencoded({ extended: false }));
app.use(cookieParser());
app.use(express.static(path.join(__dirname, 'public')));

// Reittien asettaminen
app.use('/', indexRouter);
app.use('/login',loginRouter);
app.use(authenticateToken);
app.use('/users', usersRouter);
app.use('/accounts',accountsRouter);
app.use('/addusertoaccount',accountsRouter);
app.use('/cardroutes',cardroutesRouter);
app.use('/accountinformation',accountinformationRouter);
app.use('/uploads', express.static(path.join(__dirname, 'uploads')));


// JWT-tunnistusfunktio
function authenticateToken(req, res, next) {
    const authHeader = req.headers['authorization']
    const token = authHeader && authHeader.split(' ')[1]
  
    console.log("token = "+token);
    if (token == null) return res.sendStatus(401)
  
    jwt.verify(token, process.env.MY_TOKEN, function(err, user) {
  
      if (err) return res.sendStatus(403)

      req.user = user
  
      next()
    })
  }
  app.get('/uploads/:filename', (req, res) => {

    const  filename  = req.params;
    const filePath = path.join(__dirname, 'uploads', filename);

    
        // Palautetaan tiedosto käyttämällä Expressin sendFile-metodia
        res.sendFile(filePath, (err) => {
            if (err) {
                // Käsitellään virhe, jos tiedostoa ei löydy tai muuta ongelmaa
                console.error('Virhe tiedoston palauttamisessa:', err);
                res.status(err.status || 500).end();
            }
        });
    });
    

module.exports = app;