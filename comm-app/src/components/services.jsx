import React, { useState, useEffect } from 'react';
import ForceDirectedGraph from './ForceDirectedGraph.js';
import info from "../data/result.json"
import { Table } from './Table.jsx';

export const Services = (props) => {
    const [name, setName] = useState('');
    const [k, setk] = useState('');
    const [selectedOption, setSelectedOption] = useState('');
    const [graphData, setGraphData] = useState("");
    const [error, setError] = useState(null);
    const [landingTableData, setLandingTableData] = useState({});
    useEffect(() => {
        setLandingTableData(info);
    }, []);
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
        setSelectedOption()
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
                  <table className="tablecontainer1">
                      <tr>
                          <th>
                              <div className="input-container">
                                  <label>Search name:</label>

                              </div>
                              <div className="input-box">
                                  <input className="input-text" type="text" value={name} onChange={handleNameChange} placeholder="Input search name" />
                              </div>
                              </th>
                              <th>
                              <div className="input-container">
                                  <label>K-value:</label>

                              </div>
                              <div className="input-box">
                                  <input className="input-text" type="integer" value={k} onChange={handlekChange} placeholder="Input integer" />

                                  </div>
                          </th>
                          <th>
                              <div className="input-box">
                                  <select value={selectedOption} onChange={handleOptionChange}>
                                      <option value="core">Core (available)</option>
                                      <option value="truss">Truss (available)</option>
                                      <option value="clique">Clique (unavailable)</option>
                                      <option value="ecc">Ecc (unavailable)</option>
                                  </select>
                              </div>
                          </th>
                          <th>
                              <div className="input-button">
                                  <button className="button1" onClick={(event) => handleSubmit(event)}>Submit</button>

                                  <button className="button2" type='button' onClick={handleReset}>Reset</button>
                              </div>
                          </th>
                      </tr>
                      <tr>
                              <th>
                              
                          </th>
                          
                          </tr>
                  </table>
                  <table className="tablecontainer">
                      <tr>                 
                          <th width='1000px'>
                          <div className="graph">
                                  {{ graphData } !== null ? <ForceDirectedGraph graphData={graphData} /> : null}
                                  {error && <div className="error">{error}</div>}
                              </div>
                          </th>
                          <th>
                              <Table data={landingTableData.nodes} />
                          </th>
                      </tr>
                  </table>
        </div>
      </div>
    </div>
  );
};
