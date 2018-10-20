let component = ReasonReact.statelessComponent("Card");

let make = children => {
  ...component,
  render: _self =>
    <div className="pa4 bg-alabaster br2 ba b--black-10"> ...children </div>,
};
