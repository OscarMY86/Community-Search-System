import React, { useEffect, useRef } from "react";
import WordCloud from "wordcloud";

const MyWordCloud = ({ data }) => {
    const wordCloudRef = useRef(null);

    useEffect(() => {
        if (data && data.length > 0) {
            const wordCounts = {};
            // Count the occurrences of each word in the data
            data.forEach(item => {
                const words = item.split(" ");
                words.forEach(word => {
                    wordCounts[word] = (wordCounts[word] || 0) + 1;
                });
            });

            // Convert wordCounts object to an array of objects with text and size properties
            const words = Object.keys(wordCounts).map(word => ({
                text: word,
                size: wordCounts[word]
            }));

            // Render word cloud using wordcloud library
            WordCloud(wordCloudRef.current, {
                list: words,
                gridSize: Math.round(16 * document.getElementById('wordCloudContainer').clientWidth / 1024),
                weightFactor: function (size) {
                    return Math.pow(size, 2.5) * document.getElementById('wordCloudContainer').clientWidth / 1024;
                }
            });
        }
    }, [data]);

    return <div id="wordCloudContainer" style={{ width: "100%", height: "300px" }}><canvas ref={wordCloudRef} /></div>;
};

export default MyWordCloud;
