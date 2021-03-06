let component = ReasonReact.statelessComponent("Button");

let make = (~type_="button", children) => {
  ...component,
  render: _self =>
    <button className="bw0 br2 ph3 pv2 bg-dark-blue glow white pointer" type_>
      ...children
    </button>,
};
