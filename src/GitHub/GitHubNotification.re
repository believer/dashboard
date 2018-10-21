let component = ReasonReact.statelessComponent("GitHubNotification");

let make = (~isLast, ~item: GitHub.notification, _children) => {
  ...component,
  render: _self =>
    <div className={Cn.make(["flex", "mb3"->Cn.ifTrue(!isLast)])}>
      {
        switch (item.subject.type_) {
        | PullRequest => <IconPullRequest className="mr4" />
        | VulnerabilityAlert => <IconAlertCircle className="mr4 light-red" />
        | Issue => <IconAlertCircle className="mr4" />
        | _ => ReasonReact.null
        }
      }
      <div className="lh-copy">
        <a
          className="link navy hover-hot-pink"
          href={
            item.subject.url
            |> Js.String.replace("api.", "")
            |> Js.String.replace("repos/", "")
            |> Js.String.replace("pulls/", "pull/")
          }
          target="_blank">
          {item.subject.title |> Utils.str}
        </a>
        <div className="f6 mid-gray">
          {
            item.repository.name
            ++ " "
            ++ {js|•|js}
            ++ " "
            ++ DateFns.format(item.updatedAt, "YYYY-MM-DD HH:mm")
            |> Utils.str
          }
        </div>
      </div>
    </div>,
};
