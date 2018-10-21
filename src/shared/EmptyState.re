let component = ReasonReact.statelessComponent("EmptyState");

let make = _children => {
  ...component,
  render: _self =>
    <div className="f6 mid-gray tc"> {"No notifications" |> Utils.str} </div>,
};
