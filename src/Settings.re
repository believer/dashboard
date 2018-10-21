let component = ReasonReact.statelessComponent("Settings");

let make = _children => {
  ...component,
  render: _self =>
    <div className="w-100 flex flex-column flex-row-l pv4 ph5">
      <GitHubSettings />
      <div className="mb4 mt4 bt b--black-10" />
      <TrelloSettings />
    </div>,
};
