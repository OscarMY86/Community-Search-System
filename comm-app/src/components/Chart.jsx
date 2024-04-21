import React, { useEffect, useRef } from 'react';
import { Chart, registerables } from "chart.js";

const BarChart = (props) => {
    const chartContainer = useRef(null);
    const chartInstance = useRef(null);
    Chart.register(...registerables);
    useEffect(() => {
        const counts = {};
        props.data.forEach((item) => {
            counts[item.group] = (counts[item.group] || 0) + 1;
        });

        const labels = Object.keys(counts);
        const data = Object.values(counts);

        const ctx = chartContainer.current.getContext('2d');
        chartInstance.current = new Chart(ctx, {
            type: 'bar',
            data: {
                labels: labels,
                datasets: [{
                    label: 'Count',
                    data: data,
                    backgroundColor: 'rgba(54, 162, 235, 0.6)',
                    borderColor: 'rgba(54, 162, 235, 1)',
                    borderWidth: 1
                }]
            },
            options: {
                responsive: true,
                maintainAspectRatio: false,
                scales: {
                    y: {
                        beginAtZero: true
                    }
                }
            }
        });

        // Cleanup function to destroy the chart instance when component unmounts
        return () => {
            if (chartInstance.current) {
                chartInstance.current.destroy();
            }
        };
    }, [props.data]);

    return (
        <div className="barchart">
            <canvas ref={chartContainer} width="180px" height="280" />
        </div>
    );
};

export default BarChart;
