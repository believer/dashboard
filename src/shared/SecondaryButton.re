let component = ReasonReact.statelessComponent("SecondaryButton");

let make = (~type_="button", ~onClick, children) => {
  ...component,
  render: _self =>
    <button
      className="br2 ph3 pointer bg-animate hover-bg-dark-blue hover-white pv2 bw-1 ba dark-blue b--dark-blue bg-transparent"
      onClick
      type_>
      ...children
    </button>,
};
