let component = ReasonReact.statelessComponent("Settings");

let make = _children => {
  ...component,
  render: _self =>
    <div className="w-100 pv4 ph5">
      <h1 className="mt0"> "Settings"->Utils.str </h1>
      <div className="mb4 lh-copy">
        "All values are stored on your computer and are only used for getting your notifications."
        ->Utils.str
        <div>
          "Built by "->Utils.str
          <a
            className="link dark-blue hover-hot-pink"
            href="https://github.com/believer"
            target="_blank">
            "@believer"->Utils.str
          </a>
        </div>
      </div>
      <GitHubSettings />
      <div className="mb4 mt4 bt b--black-10" />
      <TrelloSettings />
    </div>,
};
