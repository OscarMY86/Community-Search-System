import React, { useEffect, useRef } from "react";
import './ForceDirectedGraph.css';
import * as d3 from "d3";

// Reference: https://observablehq.com/@d3/force-directed-graph/2?intent=fork

// ====== This is for test1 which have grp, maybe we need to use later ======
const ForceDirectedGraph = ({ graphData }) =>{
    const svgRef = useRef(null);

    useEffect(() => {
      if (graphData){
        const width = 1000;
        const height = 600;
        const color = d3.scaleOrdinal(d3.schemeCategory10);
        
        const links = graphData.links.map(d => ({...d}));
        const nodes = graphData.nodes.map(d => ({ id: d.id  , group: d.group, name: d.name }));
        const handleMouseOver = (event, d) => {
            d3.select(event.target)
                .attr("stroke", "red") // Change border color to red
                .attr("stroke-width", 2) // Increase border width
                .attr("filter", "drop-shadow(0px 0px 5px red)"); // Add box-shadow effect
          };

        const handleMouseOut = (event, d) => {
              d3.select(event.target)
                  .attr("stroke", "#fff") // Restore original border color
                  .attr("stroke-width", 1.5) // Restore original border width
                  .attr("filter", null); // Remove box-shadow effect
        };
        const handleDoubleClick = (event, d) => {
              const url = `https://dblp.org/search?q=${encodeURIComponent(d.name)}`;
              window.open(url, '_blank');
          };
        const handleEdgeDoubleClick = (event, d) => {
              // Handle double click on edge
              const sourceName = d.source.name;
              const targetName = d.target.name;
              const url = `https://dblp.org/search?q=${encodeURIComponent(sourceName)}+${encodeURIComponent(targetName)}`;
              window.open(url, '_blank');
        };
        const simulation = d3.forceSimulation(nodes)
        .force("link", d3.forceLink(links).id(d => d.id))
        .force("charge", d3.forceManyBody())
        .force("center", d3.forceCenter(width / 2, height / 2))
        .on("tick", ticked);
        
        const svg = d3.select(svgRef.current)
        .attr("width", width)
        .attr("height", height)
        .attr("viewBox", [0, 0, width, height])
        .attr("style", "max-width: 100%; height: 100%;");
        
        const link = svg.append("g")
            .attr("stroke", "#999")
            .attr("stroke-opacity", 0.6)
            .selectAll()
            .data(links)
            .join("line")
            .attr("stroke-width", d => Math.sqrt(d.value))
            .on("mouseover", handleMouseOver)
            .on("mouseout", handleMouseOut)
            .on("dblclick", handleEdgeDoubleClick);;

        const node = svg.append("g")
            .attr("stroke", "#fff")
            .attr("stroke-width", 1.5)
            .selectAll()
            .data(nodes)
            .join("circle")
            .attr("r", 5)
            .attr("fill", d => color(d.group))
            .on("mouseover", handleMouseOver)
            .on("mouseout", handleMouseOut)
            .on("dblclick", handleDoubleClick);
            node.append("title")
            .text(d => d.name);
            
            // Add a drag behavior.
            node.call(d3.drag()
            .on("start", dragstarted)
            .on("drag", dragged)
            .on("end", dragended));
            
            // Set the position attributes of links and nodes each time the simulation ticks.
            function ticked() {
                link
                .attr("x1", d => d.source.x)
                .attr("y1", d => d.source.y)
                .attr("x2", d => d.target.x)
                .attr("y2", d => d.target.y);
                node
                .attr("cx", d => d.x)
                .attr("cy", d => d.y);
        }
        
        // Reheat the simulation when drag starts, and fix the subject position.
        function dragstarted(event) {
            if (!event.active) simulation.alphaTarget(0.3).restart();
            event.subject.fx = event.subject.x;
            event.subject.fy = event.subject.y;
        }
        
        // Update the subject (dragged node) position during drag.
        function dragged(event) {
            event.subject.fx = event.x;
            event.subject.fy = event.y;
        }
        
        // Restore the target alpha so the simulation cools after dragging ends.
        // Unfix the subject position now that it’s no longer being dragged.
        function dragended(event) {
            if (!event.active) simulation.alphaTarget(0);
            event.subject.fx = null;
            event.subject.fy = null;
        }
      }
    }, [graphData]);

    if (!graphData) {
        return null;
    }

    return <svg ref={svgRef} width={900} height={500}></svg>;
}

export default ForceDirectedGraph;
