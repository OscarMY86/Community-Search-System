import React, { useState } from 'react';
import ForceDirectedGraph from './ForceDirectedGraph.js';


export const Services = (props) => {
    const [name, setName] = useState('');
    const [k, setk] = useState('');
    const [selectedOption, setSelectedOption] = useState('');
    const [graphData, setGraphData] = useState("");
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
        setError(null);
        setGraphData(null);

        // Construct the query string
        const queryString = `name=${encodeURIComponent(name)}&k=${encodeURIComponent(k)}&option=${encodeURIComponent(selectedOption)}`;

        // Make the GET request to the Flask backend
        fetch(`http://localhost:5000/search?${queryString}`) // Search API
            // fetch(`http://localhost:5000/test?${queryString}`) // Test API
            .then((response) => response.json())
            .then((result) => {
                // Handle the response from the backend
                if (result == null) {
                    setGraphData(null);
                    setError('Invalid input');
                } else {
                    setGraphData(result);
                    setError(null);
                    console.log(result);
                }
            })
            .catch((error) => {
                setError('Make sure you input correctly.');
                setGraphData(null);
            });
    };

    const handleReset = () => {
        setName('');
        setk('');
        setSelectedOption('');
        setGraphData('');
        setError(null);
    };
  return (
    <div id="services" className="text-center">
      <div className="container">
        <div className="section-title">
                  <h2>Community Search Simulator</h2>
          <p>
                      Please enter the search name and k value to search:
          </p>
        </div>
        <div className="row">
                  <div className="input-container">
                      <label>Search name:</label>
                      
                  </div>
                  <div className="input-container">
                      <input type="text" value={name} onChange={handleNameChange} placeholder="Input search name" />
                  </div>
                  <div className="input-container">
                      <label>K-value:</label>
                      <input type="integer" value={k} onChange={handlekChange} placeholder="Input integer" />
                      <select value={selectedOption} onChange={handleOptionChange}>
                          <option value="">Please select Method</option>
                          <option value="core">Core (available)</option>
                          <option value="truss">Truss (available)</option>
                          <option value="clique">Clique (unavailable)</option>
                          <option value="ecc">Ecc (unavailable)</option>
                      </select>
                  </div>
                      <button onClick={(event) => handleSubmit(event)}>Submit</button>
                      <button type='button' onClick={handleReset}>Reset</button>
                  
                  {{ graphData } !== null ? <ForceDirectedGraph graphData={graphData} /> : null}
                  {error && <div className="error">{error}</div>}

        </div>
      </div>
    </div>
  );
};
