import './App.css';
import React, { useState} from 'react';
import ForceDirectedGraph from './ForceDirectedGraph.js';

function App() {
  const [name, setName] = useState('');
  const [k, setk] = useState('');
  const [selectedOption, setSelectedOption] = useState('');
  const [graphData, setGraphData] = useState('');
  const [error, setError] = useState(null);

  const handleNameChange = (event) => {
    setName(event.target.value);
  };

  const handlekChange = (event) => {
    const value = event.target.value;
    if (/^\d*$/.test(value)) {
      setk(value);
    }
  };
  
  const handleOptionChange = (event) => {
    setSelectedOption(event.target.value);
  };
  
  const handleSubmit = (event) => {
    event.preventDefault();

    // Construct the query string
    const queryString = `name=${encodeURIComponent(name)}&k=${encodeURIComponent(k)}&option=${encodeURIComponent(selectedOption)}`;

    // Make the GET request to the Flask backend
    // fetch(`http://localhost:5000/search?${queryString}`) // Search API
    fetch(`http://localhost:5000/test?${queryString}`) // Test API
      .then((response) => response.json())
      .then((result) => {
        // Handle the response from the backend
        if (result == null){
          setGraphData(null);
          setError('Invalid input');
        } else {
          setGraphData(result);
          setError(null);
          console.log(result);
        }
      })
      .catch((error) => {
        console.error('Error:', error);
        setError('Make sure you input correctly.');
        setGraphData(null);
      });
  };

  const handleReset = () => {
    setName('');
    setk('');
    setSelectedOption('');
  };
  
  return (
    <div className="container">
      <h1>Social Network: Community Search Project</h1>
      <p> Please enter the search name and k value to search:</p>
      <form onSubmit={handleSubmit}>
        <dic className="input-container">
          <label>Search name:</label>
          <input type="text" value={name} onChange={handleNameChange} placeholder="Input search name" />
        </dic>
        <dic className="input-container">
          <label>K-value:</label>
          <input type="integer" value={k} onChange={handlekChange} placeholder="Input integer"/> 
          <select value={selectedOption} onChange={handleOptionChange}>
            <option value="">Please select Method</option>
            <option value="core">Core</option>
            <option value="truss">Truss</option>
            <option value="clique">Clique</option>
            <option value="ecc">Ecc</option>
          </select>
        </dic>
        <button onClick={(event) => handleSubmit(event)}>Submit</button>
        <button type='button' onClick={handleReset}>Reset</button>
      </form>
      {error && <div className="error">{error}</div>}
      <div>
        {graphData !== null ? ( <ForceDirectedGraph graphData={graphData} />
        ) : (
          <div>No graph data available.</div>
        )}
      </div>
    </div>
  );
}

export default App;
