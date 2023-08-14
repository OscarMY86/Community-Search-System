import './App.css';
import React, { useState } from 'react';

function App() {
  const [name, setName] = useState('');
  const [k, setk] = useState('');

  const handleNameChange = (event) => {
    setName(event.target.value);
  };

  const handlekChange = (event) => {
    setk(event.target.value);
  };

  const handleSubmit = (event) => {
    event.preventDefault();

    // Construct the query string
    const queryString = `name=${encodeURIComponent(name)}&k=${encodeURIComponent(k)}`;

    // Make the GET request to the Flask backend
    fetch(`http://localhost:5000/search?${queryString}`)
      .then((response) => response.json())
      .then((result) => {
        // Handle the response from the backend
        console.log(result);
      })
      .catch((error) => {
        // Handle any errors
        console.error('Error:', error);
      });
  };
  
  
  return (
    <div className="container">
      <h1>Social Network: Community Search Project</h1>
      <p> Please enter the search name and k value to search:</p>

      <form onSubmit={handleSubmit}>
        <div className="input-container">
          <label>Search name:</label>
          <input type="text" value={name} onChange={handleNameChange} />
          <label>K-value:</label>
          <input type="text" value={k} onChange={handlekChange} />
          <button type="submit">Submit</button>
        </div>
      </form>
    </div>
  );
}

export default App;
