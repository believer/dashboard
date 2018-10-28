let component = ReasonReact.statelessComponent("CardError");

let make = (~errorMessage="", _children) => {
  ...component,
  render: _self =>
    <div className="tc light-red"> errorMessage->Utils.str </div>,
};
