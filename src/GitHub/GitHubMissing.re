let component = ReasonReact.statelessComponent("GitHubMissing");

let make = _children => {
  ...component,
  render: _self =>
    <div className="lh-copy">
      <div className="f5 b mb2">
        {"GitHub is missing configuration" |> Utils.str}
      </div>
      <div>
        {
          "Add the following values to your local storage. To access"
          |> Utils.str
        }
      </div>
      <ul className="list pl0">
        <li className="mb1">
          <div className="bg-light-gray dib ph2 pv1 br2">
            {"github" |> Utils.str}
          </div>
          {" - generate a token " |> Utils.str}
          <a
            className="link blue hover-hot-pink"
            href="https://github.com/settings/tokens">
            {"here" |> Utils.str}
          </a>
        </li>
      </ul>
      <div>
        {"Token needs to have scope " |> Utils.str}
        <strong> {"notifications" |> Utils.str} </strong>
        {
          " and if you want to access private repositories it also needs scope "
          |> Utils.str
        }
        <strong> {"repo" |> Utils.str} </strong>
        {"." |> Utils.str}
      </div>
    </div>,
};
