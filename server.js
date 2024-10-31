const express = require('express');
const cors = require('cors'); // Import CORS middleware
const app = express();
const PORT = 3000;

// Enable CORS to allow requests from different origins (like your frontend on port 8080)
app.use(cors());

// Serve static files (HTML, CSS, JS)
app.use(express.static('public'));

// Example API endpoint for sensor data
app.get('/api/sensors', (req, res) => {
    const sensorData = {
        room1: 'No Smoke',
        room2: 'Smoke Detected',
        room3: 'All Clear',
    };
    res.json(sensorData);
});

// Start the server
app.listen(PORT, () => {
    console.log(`Server running at http://localhost:${PORT}`);
});
