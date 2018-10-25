let component = ReasonReact.statelessComponent("CardLoading");

let make = _children => {
  ...component,
  render: _self => <div className="tc mid-gray"> "Loading"->Utils.str </div>,
};
