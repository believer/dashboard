let component = ReasonReact.statelessComponent("Button");

let make = (~type_="button", children) => {
  ...component,
  render: _self =>
    <button className="bw0 br2 ph3 pv2 bg-navy white" type_>
      ...children
    </button>,
};
