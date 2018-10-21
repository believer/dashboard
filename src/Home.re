let component = ReasonReact.statelessComponent("Home");

let make = _children => {
  ...component,

  render: _self =>
    <div className="w-100 flex flex-column flex-row-l pv4 ph5">
      <GitHubContainer />
      <TrelloContainer />
    </div>,
};
