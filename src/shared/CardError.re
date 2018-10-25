let component = ReasonReact.statelessComponent("CardError");

let make = _children => {
  ...component,
  render: _self =>
    <div className="tc light-red">
      "Something went wrong. Are the settings up to date?"->Utils.str
    </div>,
};
