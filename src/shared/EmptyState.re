let component = ReasonReact.statelessComponent("EmptyState");

let make = _children => {
  ...component,
  render: _self =>
    <div className="f6 light-silver tc">
      {"No notifications" |> Utils.str}
    </div>,
};
