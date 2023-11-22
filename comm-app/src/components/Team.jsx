import React, { useState } from "react";
import ForceDirectedGraph from './ForceDirectedGraph.js';

export const Team = (props) => {
    const [name, setName] = useState('');
    const [k1, setk1] = useState('');
    const [k2, setk2] = useState('');
    const [graphData1, setGraphData1] = useState("");
    const [graphData2, setGraphData2] = useState("");
    const [error, setError] = useState(null);

    const handleNameChange = (event) => {
        setName(event.target.value);
    };

    const handlek1Change = (event) => {
        const value = event.target.value;
        if (/^\d*$/.test(value)) {
            setk1(value);
        }
    };

    const handlek2Change = (event) => {
        const value = event.target.value;
        if (/^\d*$/.test(value)) {
            setk2(value);
        }
    };
    const handleSubmit = (event) => {
       
        event.preventDefault();
        setError(null);
        setGraphData1(null);
        setGraphData2(null);

        // Construct the query string
        const queryString1 = `name=${encodeURIComponent(name)}&k=${encodeURIComponent(k1)}&option=${encodeURIComponent('core')}`;
        const queryString2 = `name=${encodeURIComponent(name)}&k=${encodeURIComponent(k2)}&option=${encodeURIComponent('truss')}`;
        // Make the GET request to the Flask backend
        fetch(`http://localhost:5000/search?${queryString1}`) // Search API
            // fetch(`http://localhost:5000/test?${queryString}`) // Test API
            .then((response) => response.json())
            .then((result) => {
                // Handle the response from the backend
                if (result == null) {
                    setGraphData1(null);
                    setError('Invalid input');
                } else {
                    setGraphData1(result);
                    setError(null);
                    console.log(result);
                }
            })
            .catch((error) => {
                setError('Make sure you input correctly.');
                setGraphData1(null);
            });
        fetch(`http://localhost:5000/search?${queryString2}`) // Search API
            // fetch(`http://localhost:5000/test?${queryString}`) // Test API
            .then((response) => response.json())
            .then((result) => {
                // Handle the response from the backend
                if (result == null) {
                    setGraphData2(null);
                    setError('Invalid input');
                } else {
                    setGraphData2(result);
                    setError(null);
                    console.log(result);
                }
            })
            .catch((error) => {
                setError('Make sure you input correctly.');
                setGraphData1(null);
            });
    };

    const handleReset = () => {
        setName('');
        setk1('');
        setk2('');
        setGraphData1('');
        setGraphData2('');
        setError(null);
    };
  return (
    <div id="team" className="text-center">
      <div className="container">
        <div className="col-md-8 col-md-offset-2 section-title">
          <h2>Comparison</h2>
          <p>
            We will compare k-core and k-truss
          </p>
        </div>
        <div id="row">
                  <table className="tablecontainer1">
                      <tr>
                          <th>
                              <div className="input-container2">
                                  <label>Search name:</label>

                              </div>
                              <div className="input-box">
                                  <input className="input-text" type="text" value={name} onChange={handleNameChange} placeholder="Input search name" />
                              </div>
                          </th>
                          <th>
                              <div className="input-container2">
                                  <label>K-core value:</label>

                              </div>
                              <div className="input-box">
                                  <input className="input-text" type="integer" value={k1} onChange={handlek1Change} placeholder="Input integer" />

                              </div>
                          </th>
                          <th>
                              <div className="input-container2">
                                  <label>K-truss value:</label>

                              </div>
                              <div className="input-box">
                                  <input className="input-text" type="integer" value={k2} onChange={handlek2Change} placeholder="Input integer" />

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
                  <table className= "tablecontainer">
                      <tr>
                          <th>
                              <div className="input-container3">
                                  <label>k core</label>

                              </div>
                              <div className="graph1">
                                  {{ graphData1 } !== null ? <ForceDirectedGraph graphData={graphData1} /> : null}
                                  {error && <div className="error">{error}</div>}
                              </div>
                          </th>
                          <th>
                          </th>

                          <th>
                              <div className="input-container3">
                                  <label>k truss</label>

                              </div>
                              <div className="graph1">
                                  {{ graphData2 } !== null ? <ForceDirectedGraph graphData={graphData2} /> : null}
                                  {error && <div className="error">{error}</div>}
                              </div>
                          </th>
                          <th>
                          </th>
                      </tr>
                  </table>
        </div>
      </div>
    </div>
  );
};
