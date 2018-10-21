let component = ReasonReact.statelessComponent("GitHubMissing");

let make = _children => {
  ...component,
  render: _self =>
    <div className="lh-copy">
      <div className="f5 b mb2">
        {"GitHub is missing configuration" |> Utils.str}
      </div>
      <div>
        {"Set the values in " |> Utils.str}
        <Link href="/#/settings"> "/settings"->Utils.str </Link>
      </div>
    </div>,
};
