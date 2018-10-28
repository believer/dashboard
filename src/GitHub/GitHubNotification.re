let component = ReasonReact.statelessComponent("GitHubItem");

module MarkAsRead = [%graphql
  {|
    mutation gitHubMarkAsRead($id: ID!) {
      gitHubMarkAsRead(id: $id)
    }
|}
];

module MarkAsReadMutation = ReasonApollo.CreateMutation(MarkAsRead);

let make = (~isLast, ~item, _children) => {
  ...component,
  render: _self =>
    <MarkAsReadMutation>
      ...{
           (mutation, _) => {
             let markAsRead = MarkAsRead.make(~id=item##id, ());

             <div className={Cn.make(["flex", "mb3"->Cn.ifTrue(!isLast)])}>
               <div className="mr4">
                 {
                   switch (item##subject##type_) {
                   | "Commit" => <IconGitCommit />
                   | "PullRequest" => <IconPullRequest />
                   | "VulnerabilityAlert" =>
                     <IconAlertCircle className="light-red" />
                   | "Issue" => <IconAlertCircle />
                   | _ => ReasonReact.null
                   }
                 }
                 <IconCheckCircle
                   className="pointer mt2 green"
                   onClick={
                     _ =>
                       mutation(
                         ~variables=markAsRead##variables,
                         ~refetchQueries=[|"gitHubNotifications"|],
                         (),
                       )
                       |> ignore
                   }
                 />
               </div>
               <div className="lh-copy">
                 <a
                   className="link dark-blue hover-hot-pink"
                   href={
                     item##subject##url
                     |> Js.String.replace("api.", "")
                     |> Js.String.replace("repos/", "")
                     |> Js.String.replace("pulls/", "pull/")
                   }
                   target="_blank">
                   {item##subject##title |> Utils.str}
                 </a>
                 <div className="f6 mid-gray">
                   {
                     item##repository##name
                     ++ " "
                     ++ {js|•|js}
                     ++ " "
                     ++ DateFns.format(item##date, "HH:mm")
                     |> Utils.str
                   }
                 </div>
               </div>
             </div>;
           }
         }
    </MarkAsReadMutation>,
};
