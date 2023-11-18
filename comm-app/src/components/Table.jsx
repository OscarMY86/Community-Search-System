import React from "react";

export const Table = (props) => {
    return (
        <div className="wrapper">
            <table class="card-list-table" >
                    <tr className="table_head">
                        <th className="table_style">Id</th>
                        <th className="table_style">Name</th>
                        <th className="table_style">Community</th>
                        <th className="table_style">Neighbor Number</th>
                        <th className="table_style">Neighbor Distance</th>
                </tr>
                {props.data ? props.data.map((d, i) => (
                    <tr className="table_body">
                        <th className="table_style">{d.id}</th>
                        <th className="table_style">{d.name}</th>
                        <th className="table_style">{d.group}</th>
                        <th className="table_style">{d.neighbor_number}</th>
                        <th className="table_style">{d.neighbor_distance}</th>
                    </tr>)) : "Loading..."}
            </table>
        </div>
    )
};