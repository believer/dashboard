let component = ReasonReact.statelessComponent("GitHubNotification");

let make = (~isLast, ~item: GitHub.notification, _children) => {
  ...component,
  render: _self =>
    <div className={Cn.make(["flex", "mb3"->Cn.ifTrue(!isLast)])}>
      <IconPullRequest className="mr4" />
      <div>
        <a
          className="link blue hover-hot-pink"
          href={item.subject.url}
          target="_blank">
          {item.subject.title |> Utils.str}
        </a>
        <div className="f6 light-silver mt2">
          {
            MomentRe.moment(item.updatedAt)
            |> MomentRe.Moment.format("YYYY-MM-DD HH:mm")
            |> Utils.str
          }
        </div>
      </div>
    </div>,
};