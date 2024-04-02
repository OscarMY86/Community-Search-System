import React, { useState, useEffect } from 'react';
import ForceDirectedGraph from './ForceDirectedGraph.js';
import info from "../data/result.json"
import { Table } from './Table.jsx';
import { Swiper, SwiperSlide } from 'swiper/react';
import 'swiper/css';
import 'swiper/css/navigation';
import { Navigation } from 'swiper/modules';

export const Services = (props) => {
    const [name, setName] = useState('');
    const [GNNname, setGNNName] = useState('');
    const [k, setk] = useState('');
    const [selectedOption, setSelectedOption] = useState('core');
    const [selectedGNNOption, setSelectedGNNOption] = useState('BFSO');
    const [graphData, setGraphData] = useState("");
    const [GNNgraphData, setGNNGraphData] = useState("");
    const [error, setError] = useState(null);
    const [gnnerror, setGNNError] = useState(null);
    const [landingTableData, setLandingTableData] = useState({});
    const [limit, setLimit] = useState('100');
    const [Add, handleAdd] = useState(false);
    const [swiperEnabled] = useState(false);

    useEffect(() => {
        setLandingTableData(info);
    }, []);

    const handleNameChange = (event) => {
        setName(event.target.value);
    };
    const handleGNNNameChange = (event) => {
        setGNNName(event.target.value);
    };

    const handlekChange = (event) => {
        const value = event.target.value;
        if (/^\d*$/.test(value)) {
            setk(value);
        }
    };

    const handleLimitChange = (event) => {
        setLimit(event.target.value);
    };

    const handleOptionChange = (event) => {
        setSelectedOption(event.target.value);
    };
    const handleGNNOptionChange = (event) => {
        setSelectedGNNOption(event.target.value);
    };
    const handleDelete = (event) => {

    };

    const handleSubmit = (event) => {
        event.preventDefault();
        setError(null);
        setGraphData(null);

        // Construct the query string
        const queryString = `name=${encodeURIComponent(name)}&k=${encodeURIComponent(k)}&option=${encodeURIComponent(selectedOption)}&limit=${encodeURIComponent(limit)}`;

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

    const handleGNNSubmit = (event) => {
        event.preventDefault();
        setGNNError(null);
        setGNNGraphData(null);

        // Construct the query string
        const queryGNNString = `name=${encodeURIComponent(GNNname)}&option=${encodeURIComponent(selectedGNNOption)}`;

        // Make the GET request to the Flask backend
        fetch(`http://localhost:8081/search?${queryGNNString}`,{
            method: "GET", 
            mode: "cors",
        }) // Search API
            .then((response) => response.json())
            .then((result) => {
                // Handle the response from the backend
                if (result == null) {
                    setGNNGraphData(null);
                    setGNNError('Invalid input');
                } else {
                    setGNNGraphData(result);
                    setGNNError(null);
                    console.log(result);
                }
            })
            .catch((gnnerror) => {
                setGNNError('Make sure you input correctly for GNN model.');
                setGNNGraphData(null);
            });
    };
    const handleReset = () => {
        setName('');
        setk('');
        setSelectedOption('');
        setGraphData('');
        setError(null);
    };
    const handleGNNReset = () => {
        setGNNName('');
        setSelectedGNNOption('');
        setGNNGraphData('');
        setGNNError(null);
    };
    const handleSave = (event) => {
        console.log(graphData);
        if (graphData) {
            const queryString = `name=${encodeURIComponent(name)}&k=${encodeURIComponent(k)}&option=${encodeURIComponent(selectedOption)}&limit=${encodeURIComponent(limit)}`
            fetch(`http://localhost:5000/save?${queryString}`)
                .then((response) => response.json())
                .then((result) => {
                    console.log('Data saved successfully:', result);
                })
                .catch(error => {
                    console.error('Error saving data:', error);
                });
        }
    };
    return (

        <div id="services" className="text-center">
            <Swiper navigation={true} modules={[Navigation]} className="mySwiper" touchEventsTarget="container" allowTouchMove={swiperEnabled}>
                <SwiperSlide>
                    <div className="container">
                        <div className="section-title">
                            <h2>Community Search Simulator</h2>
                            <p>
                                Please enter the search name and k value to search:
                            </p>
                        </div>

                        <div className="row_table">
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
                                        <div className="input-container">
                                            <label>Limit:</label>
                                        </div>
                                        <div className="input-box">
                                            <input className="input-text" type="integer" value={limit} onChange={handleLimitChange} placeholder="Input limit" />
                                        </div>
                                    </th>
                                    <th>
                                        <div className="input-box">
                                            <select value={selectedOption} onChange={handleOptionChange}>
                                                <option value="core">Core</option>
                                                <option value="truss">Truss</option>
                                                {/* <option value="clique">Clique (unavailable)</option> */}
                                                {/* <option value="ecc">Ecc (unavailable)</option> */}
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
                                        <button className="button5" type='button' onClick={handleSave}>Save</button>
                                        <Table data={landingTableData.nodes} />
                                    </th>
                                </tr>
                            </table>
                        </div>
                    </div>
                </SwiperSlide>
                <SwiperSlide>
                    <div className="container">
                        <div className="section-title">
                            <h2>Community Search Simulator</h2>
                            <p>
                                Please enter the search name and k value to search:
                            </p>
                        </div>

                        <div className="row_table">
                            <table className="tablecontainer1">
                                <tr>
                                    <th>
                                        <div className="input-container">
                                            <label>Search name:</label>
                                        </div>
                                        <div className="input-box">
                                            <input className="input-text" type="text" value={GNNname} onChange={handleGNNNameChange} placeholder="Input search name" />
                                        </div>
                                    </th>
                                    <th>
                                        <div className="input-box">
                                            <select value={selectedGNNOption} onChange={handleGNNOptionChange}>
                                                <option value="BFSO">BSF only</option>
                                                <option value="BFSS">BSF swap</option>
                                                <option value="GreedyT">Greedy-T</option>
                                                <option value="GreedyG">Greedy-G</option>
                                            </select>
                                        </div>
                                    </th>
                                    <th>
                                        <div className="input-button">
                                            <button className="button1" onClick={(event) => handleGNNSubmit(event)}>Submit</button>

                                            <button className="button2" type='button' onClick={handleGNNReset}>Reset</button>

                                            {/* <button className="button4" type='button' onClick={() => handleAdd(!Add)}>{Add ? "^" : "+"}</button> */}

                                        </div>
                                    </th>
                                </tr>
                            </table>
                            {Add && <table>
                                <tr>
                                    <th>
                                        <div className="input-container">
                                            <label>Name:</label>
                                        </div>
                                        <div className="input-box">
                                            <input className="input-text" type="text" value={GNNname} onChange={handleGNNNameChange} placeholder="Input search name" />
                                        </div>
                                    </th>
                                    <th>
                                        <div className="input-button">
                                            <button className="button1" onClick={(event) => handleGNNSubmit(event)}>Submit</button>
                                        </div>
                                    </th>
                                </tr>
                            </table>}
                            <table className="tablecontainer">
                                <tr>
                                    <th width='1000px'>
                                        <div className="graph">
                                            {{ GNNgraphData } !== null ? <ForceDirectedGraph graphData={GNNgraphData} /> : null}
                                            {gnnerror && <div className="error">{gnnerror}</div>}
                                        </div>
                                    </th>
                                    <th>
                                        {/* <button className="button5" type='button' onClick={handleSave}>Save</button>
                                        <button className="button3" type='button' onClick={handleDelete}>Delete</button> */}
                                        {/* <Table data={landingTableData.nodes} /> */}
                                    </th>
                                </tr>
                            </table>
                        </div>
                    </div>
                </SwiperSlide>
            </Swiper>
        </div>
    );
};