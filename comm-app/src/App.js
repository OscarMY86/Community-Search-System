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

  
  return (
    <div className="container">
      <h1>Social Network: Community Search Project</h1>
      <p> Please enter the search name and k value to search:</p>

      <div className="input-container">
        <label>Search name:</label>
        <input type="text" value={name} onChange={handleNameChange} />
        <label>K-value:</label>
        <input type="text" value={k} onChange={handlekChange} />
      </div>
    </div>
  );
}

export default App;
